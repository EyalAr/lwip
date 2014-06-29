// tga2jpg.cpp - jpge/jpgd example command line app.
// Public domain, Rich Geldreich <richgel99@gmail.com>
// Last updated May. 19, 2012

// Note: jpge.cpp/h and jpgd.cpp/h are completely standalone, i.e. they do not have any dependencies to each other.
#include "jpge.h"
#include "jpgd.h"
#include "stb_image.c"
#include "timer.h"
#include <ctype.h>

#if defined(_MSC_VER)
  #define strcasecmp _stricmp
#else
  #define strcpy_s(d, c, s) strcpy(d, s)
#endif

static int print_usage()
{
  printf("Usage: jpge [options] <source_file> <dest_file> <quality_factor>\n");
  printf("\nRequired parameters (must follow options):\n");
  printf("source_file: Source image file, in any format stb_image.c supports.\n");
  printf("dest_file: Destination JPEG file.\n");
  printf("quality_factor: 1-100, higher=better (only needed in compression mode)\n");
  printf("\nDefault mode compresses source_file to dest_file. Alternate modes:\n");
  printf("-x: Exhaustive compression test (only needs source_file)\n");
  printf("-d: Test jpgd.h. source_file must be JPEG, and dest_file must be .TGA\n");
  printf("\nOptions supported in all modes:\n");
  printf("-glogfilename.txt: Append output to log file\n");
  printf("\nOptions supported in compression mode (the default):\n");
  printf("-o: Enable optimized Huffman tables (slower, but smaller files)\n");
  printf("-luma: Output Y-only image\n");
  printf("-h1v1, -h2v1, -h2v2: Chroma subsampling (default is either Y-only or H2V2)\n");
  printf("-m: Test mem to mem compression (instead of mem to file)\n");
  printf("-wfilename.tga: Write decompressed image to filename.tga\n");
  printf("-s: Use stb_image.c to decompress JPEG image, instead of jpgd.cpp\n");
  printf("\nExample usages:\n");
  printf("Test compression: jpge orig.png comp.jpg 90\n");
  printf("Test decompression: jpge -d comp.jpg uncomp.tga\n");
  printf("Exhaustively test compressor: jpge -x orig.png\n");
  
  return EXIT_FAILURE;
}

static char s_log_filename[256];

static void log_printf(const char *pMsg, ...)
{
  va_list args;

  va_start(args, pMsg);
  char buf[2048];
  vsnprintf(buf, sizeof(buf) - 1, pMsg, args);
  buf[sizeof(buf) - 1] = '\0';
  va_end(args);

  printf("%s", buf);

  if (s_log_filename[0])
  {
    FILE *pFile = fopen(s_log_filename, "a+");
    if (pFile)
    {
      fprintf(pFile, "%s", buf);
      fclose(pFile);
    }
  }
}

static uint get_file_size(const char *pFilename)
{
  FILE *pFile = fopen(pFilename, "rb");
  if (!pFile) return 0;
  fseek(pFile, 0, SEEK_END);
  uint file_size = ftell(pFile);
  fclose(pFile);
  return file_size;
}

struct image_compare_results
{
  image_compare_results() { memset(this, 0, sizeof(*this)); }

  double max_err;
  double mean;
  double mean_squared;
  double root_mean_squared;
  double peak_snr;
};

static void get_pixel(int* pDst, const uint8 *pSrc, bool luma_only, int num_comps)
{
  int r, g, b;
  if (num_comps == 1)
  {
    r = g = b = pSrc[0];
  }
  else if (luma_only)
  {
    const int YR = 19595, YG = 38470, YB = 7471;
    r = g = b = (pSrc[0] * YR + pSrc[1] * YG + pSrc[2] * YB + 32768) / 65536;
  }
  else
  {
    r = pSrc[0]; g = pSrc[1]; b = pSrc[2];
  }
  pDst[0] = r; pDst[1] = g; pDst[2] = b;
}

// Compute image error metrics.
static void image_compare(image_compare_results &results, int width, int height, const uint8 *pComp_image, int comp_image_comps, const uint8 *pUncomp_image_data, int uncomp_comps, bool luma_only)
{
  double hist[256];
  memset(hist, 0, sizeof(hist));

  const uint first_channel = 0, num_channels = 3;
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      int a[3]; get_pixel(a, pComp_image + (y * width + x) * comp_image_comps, luma_only, comp_image_comps);
      int b[3]; get_pixel(b, pUncomp_image_data + (y * width + x) * uncomp_comps, luma_only, uncomp_comps);
      for (uint c = 0; c < num_channels; c++)
        hist[labs(a[first_channel + c] - b[first_channel + c])]++;
    }
  }

  results.max_err = 0;
  double sum = 0.0f, sum2 = 0.0f;
  for (uint i = 0; i < 256; i++)
  {
    if (!hist[i])
      continue;
    if (i > results.max_err)
      results.max_err = i;
    double x = i * hist[i];
    sum += x;
    sum2 += i * x;
  }

  // See http://bmrc.berkeley.edu/courseware/cs294/fall97/assignment/psnr.html
  double total_values = width * height;

  results.mean = sum / total_values;
  results.mean_squared = sum2 / total_values;

  results.root_mean_squared = sqrt(results.mean_squared);

  if (!results.root_mean_squared)
    results.peak_snr = 1e+10f;
  else
    results.peak_snr = log10(255.0f / results.root_mean_squared) * 20.0f;
}

// Simple exhaustive test. Tries compressing/decompressing image using all supported quality, subsampling, and Huffman optimization settings.
static int exhausive_compression_test(const char *pSrc_filename, bool use_jpgd)
{
  int status = EXIT_SUCCESS;

  // Load the source image.
  const int req_comps = 3; // request RGB image
  int width = 0, height = 0, actual_comps = 0;
  uint8 *pImage_data = stbi_load(pSrc_filename, &width, &height, &actual_comps, req_comps);
  if (!pImage_data)
  {
    log_printf("Failed loading file \"%s\"!\n", pSrc_filename);
    return EXIT_FAILURE;
  }

  log_printf("Source file: \"%s\" Image resolution: %ix%i Actual comps: %i\n", pSrc_filename, width, height, actual_comps);

  int orig_buf_size = width * height * 3; // allocate a buffer that's hopefully big enough (this is way overkill for jpeg)
  if (orig_buf_size < 1024) orig_buf_size = 1024;
  void *pBuf = malloc(orig_buf_size);

  uint8 *pUncomp_image_data = NULL;

  double max_err = 0;
  double lowest_psnr = 9e+9;
  double threshold_psnr = 9e+9;
  double threshold_max_err = 0.0f;

  image_compare_results prev_results;

  for (uint quality_factor = 1; quality_factor <= 100; quality_factor++)
  {
    for (uint subsampling = 0; subsampling <= jpge::H2V2; subsampling++)
    {
      for (uint optimize_huffman_tables = 0; optimize_huffman_tables <= 1; optimize_huffman_tables++)
      {
        // Fill in the compression parameter structure.
        jpge::params params;
        params.m_quality = quality_factor;
        params.m_subsampling = static_cast<jpge::subsampling_t>(subsampling);
        params.m_two_pass_flag = (optimize_huffman_tables != 0);

        int comp_size = orig_buf_size;
        if (!jpge::compress_image_to_jpeg_file_in_memory(pBuf, comp_size, width, height, req_comps, pImage_data, params))
        {
          status = EXIT_FAILURE;
          goto failure;
        }

        int uncomp_width = 0, uncomp_height = 0, uncomp_actual_comps = 0, uncomp_req_comps = 3;
        free(pUncomp_image_data);
        if (use_jpgd)
          pUncomp_image_data = jpgd::decompress_jpeg_image_from_memory((const stbi_uc*)pBuf, comp_size, &uncomp_width, &uncomp_height, &uncomp_actual_comps, uncomp_req_comps);
        else
          pUncomp_image_data = stbi_load_from_memory((const stbi_uc*)pBuf, comp_size, &uncomp_width, &uncomp_height, &uncomp_actual_comps, uncomp_req_comps);
        if (!pUncomp_image_data)
        {
          status = EXIT_FAILURE;
          goto failure;
        }

        if ((uncomp_width != width) || (uncomp_height != height))
        {
          status = EXIT_FAILURE;
          goto failure;
        }

        image_compare_results results;
        image_compare(results, width, height, pImage_data, req_comps, pUncomp_image_data, uncomp_req_comps, (params.m_subsampling == jpge::Y_ONLY) || (actual_comps == 1) || (uncomp_actual_comps == 1));
        //log_printf("Q: %3u, S: %u, O: %u, CompSize: %7u, Error Max: %3.3f, Mean: %3.3f, Mean^2: %5.3f, RMSE: %3.3f, PSNR: %3.3f\n", quality_factor, subsampling, optimize_huffman_tables, comp_size, results.max_err, results.mean, results.mean_squared, results.root_mean_squared, results.peak_snr);
        log_printf("%3u, %u, %u, %7u, %3.3f, %3.3f, %5.3f, %3.3f, %3.3f\n", quality_factor, subsampling, optimize_huffman_tables, comp_size, results.max_err, results.mean, results.mean_squared, results.root_mean_squared, results.peak_snr);
        if (results.max_err > max_err) max_err = results.max_err;
        if (results.peak_snr < lowest_psnr) lowest_psnr = results.peak_snr;

        if (quality_factor == 1)
        {
          if (results.peak_snr < threshold_psnr)
            threshold_psnr = results.peak_snr;
          if (results.max_err > threshold_max_err)
            threshold_max_err = results.max_err;
        }
        else
        {
          // Couple empirically determined tests - worked OK on my test data set.
          if ((results.peak_snr < (threshold_psnr - 3.0f)) || (results.peak_snr < 6.0f)) 
          {
            status = EXIT_FAILURE;
            goto failure;
          }
          if (optimize_huffman_tables)
          {
            if ((prev_results.max_err != results.max_err) || (prev_results.peak_snr != results.peak_snr))
            {
              status = EXIT_FAILURE;
              goto failure;
            }
          }
        }

        prev_results = results;
      }
    }
  }

  log_printf("Max error: %f Lowest PSNR: %f\n", max_err, lowest_psnr);

failure:
  free(pImage_data);
  free(pBuf);
  free(pUncomp_image_data);

  log_printf((status == EXIT_SUCCESS) ? "Success.\n" : "Exhaustive test failed!\n");
  return status;
}

// Test JPEG file decompression using jpgd.h
static int test_jpgd(const char *pSrc_filename, const char *pDst_filename)
{
  // Load the source JPEG image.
  const int req_comps = 3; // request RGB image
  int width = 0, height = 0, actual_comps = 0;
  
  timer tm;
  tm.start();

  uint8 *pImage_data = jpgd::decompress_jpeg_image_from_file(pSrc_filename, &width, &height, &actual_comps, req_comps);

  tm.stop();

  if (!pImage_data)
  {
    log_printf("Failed loading JPEG file \"%s\"!\n", pSrc_filename);
    return EXIT_FAILURE;
  }
    
  log_printf("Source JPEG file: \"%s\", image resolution: %ix%i, actual comps: %i\n", pSrc_filename, width, height, actual_comps);
  
  log_printf("Decompression time: %3.3fms\n", tm.get_elapsed_ms());

  if (!stbi_write_tga(pDst_filename, width, height, req_comps, pImage_data))
  {
    log_printf("Failed writing image to file \"%s\"!\n", pDst_filename);
    free(pImage_data);
    return EXIT_FAILURE;
  }
  log_printf("Wrote decompressed image to TGA file \"%s\"\n", pDst_filename);
  
  log_printf("Success.\n");

  free(pImage_data);
  return EXIT_SUCCESS;
}

int main(int arg_c, char* ppArgs[])
{
  printf("jpge/jpgd example app\n");

  // Parse command line.
  bool run_exhausive_test = false;
  bool test_memory_compression = false;
  bool optimize_huffman_tables = false;
  int subsampling = -1;
  char output_filename[256] = "";
  bool use_jpgd = true;
  bool test_jpgd_decompression = false;

  int arg_index = 1;
  while ((arg_index < arg_c) && (ppArgs[arg_index][0] == '-'))
  {
    switch (tolower(ppArgs[arg_index][1]))
    {
    case 'd':
      test_jpgd_decompression = true;
      break;
    case 'g':
      strcpy_s(s_log_filename, sizeof(s_log_filename), &ppArgs[arg_index][2]);
      break;
    case 'x':
      run_exhausive_test = true;
      break;
    case 'm':
      test_memory_compression = true;
      break;
    case 'o':
      optimize_huffman_tables = true;
      break;
    case 'l':
      if (strcasecmp(&ppArgs[arg_index][1], "luma") == 0)
        subsampling = jpge::Y_ONLY;
      else
      {
        log_printf("Unrecognized option: %s\n", ppArgs[arg_index]);
        return EXIT_FAILURE;
      }
      break;
    case 'h':
      if (strcasecmp(&ppArgs[arg_index][1], "h1v1") == 0)
        subsampling = jpge::H1V1;
      else if (strcasecmp(&ppArgs[arg_index][1], "h2v1") == 0)
        subsampling = jpge::H2V1;
      else if (strcasecmp(&ppArgs[arg_index][1], "h2v2") == 0)
        subsampling = jpge::H2V2;
      else
      {
        log_printf("Unrecognized subsampling: %s\n", ppArgs[arg_index]);
        return EXIT_FAILURE;
      }
      break;
    case 'w':
    {
      strcpy_s(output_filename, sizeof(output_filename), &ppArgs[arg_index][2]);
      break;
    }
    case 's':
    {
      use_jpgd = false;
      break;
    }
    default:
      log_printf("Unrecognized option: %s\n", ppArgs[arg_index]);
      return EXIT_FAILURE;
    }
    arg_index++;
  }

  if (run_exhausive_test)
  {
    if ((arg_c - arg_index) < 1)
    {
      log_printf("Not enough parameters (expected source file)\n");
      return print_usage();
    }

    const char* pSrc_filename = ppArgs[arg_index++];
    return exhausive_compression_test(pSrc_filename, use_jpgd);
  }
  else if (test_jpgd_decompression)
  {
    if ((arg_c - arg_index) < 2)
    {
      log_printf("Not enough parameters (expected source and destination files)\n");
      return print_usage();
    }

    const char* pSrc_filename = ppArgs[arg_index++];
    const char* pDst_filename = ppArgs[arg_index++];
    return test_jpgd(pSrc_filename, pDst_filename);
  }

  // Test jpge
  if ((arg_c - arg_index) < 3)
  {
    log_printf("Not enough parameters (expected source file, dest file, quality factor to follow options)\n");
    return print_usage();
  }

  const char* pSrc_filename = ppArgs[arg_index++];
  const char* pDst_filename = ppArgs[arg_index++];

  int quality_factor = atoi(ppArgs[arg_index++]);
  if ((quality_factor < 1) || (quality_factor > 100))
  {
    log_printf("Quality factor must range from 1-100!\n");
    return EXIT_FAILURE;
  }

  // Load the source image.
  const int req_comps = 3; // request RGB image
  int width = 0, height = 0, actual_comps = 0;
  uint8 *pImage_data = stbi_load(pSrc_filename, &width, &height, &actual_comps, req_comps);
  if (!pImage_data)
  {
    log_printf("Failed loading file \"%s\"!\n", pSrc_filename);
    return EXIT_FAILURE;
  }

  log_printf("Source file: \"%s\", image resolution: %ix%i, actual comps: %i\n", pSrc_filename, width, height, actual_comps);

  // Fill in the compression parameter structure.
  jpge::params params;
  params.m_quality = quality_factor;
  params.m_subsampling = (subsampling < 0) ? ((actual_comps == 1) ? jpge::Y_ONLY : jpge::H2V2) : static_cast<jpge::subsampling_t>(subsampling);
  params.m_two_pass_flag = optimize_huffman_tables;

  log_printf("Writing JPEG image to file: %s\n", pDst_filename);

  timer tm;
  
  // Now create the JPEG file.
  if (test_memory_compression)
  {
    int buf_size = width * height * 3; // allocate a buffer that's hopefully big enough (this is way overkill for jpeg)
    if (buf_size < 1024) buf_size = 1024;
    void *pBuf = malloc(buf_size);

    tm.start();
    if (!jpge::compress_image_to_jpeg_file_in_memory(pBuf, buf_size, width, height, req_comps, pImage_data, params))
    {
       log_printf("Failed creating JPEG data!\n");
       return EXIT_FAILURE;
    }
    tm.stop();

    FILE *pFile = fopen(pDst_filename, "wb");
    if (!pFile)
    {
       log_printf("Failed creating file \"%s\"!\n", pDst_filename);
       return EXIT_FAILURE;
    }

    if (fwrite(pBuf, buf_size, 1, pFile) != 1)
    {
       log_printf("Failed writing to output file!\n");
       return EXIT_FAILURE;
    }

    if (fclose(pFile) == EOF)
    {
       log_printf("Failed writing to output file!\n");
       return EXIT_FAILURE;
    }
  }
  else
  {
    tm.start();

    if (!jpge::compress_image_to_jpeg_file(pDst_filename, width, height, req_comps, pImage_data, params))
    {
       log_printf("Failed writing to output file!\n");
       return EXIT_FAILURE;
    }
    tm.stop();
  }

  double total_comp_time = tm.get_elapsed_ms();

  const uint comp_file_size = get_file_size(pDst_filename);
  const uint total_pixels = width * height;
  log_printf("Compressed file size: %u, bits/pixel: %3.3f\n", comp_file_size, (comp_file_size * 8.0f) / total_pixels);

  // Now try loading the JPEG file using jpgd or stbi_image's JPEG decompressor.
  int uncomp_width = 0, uncomp_height = 0, uncomp_actual_comps = 0, uncomp_req_comps = 3;

  tm.start();
  uint8 *pUncomp_image_data;
  if (use_jpgd)
    pUncomp_image_data = jpgd::decompress_jpeg_image_from_file(pDst_filename, &uncomp_width, &uncomp_height, &uncomp_actual_comps, uncomp_req_comps);
  else
    pUncomp_image_data = stbi_load(pDst_filename, &uncomp_width, &uncomp_height, &uncomp_actual_comps, uncomp_req_comps);

  double total_uncomp_time = tm.get_elapsed_ms();

  if (!pUncomp_image_data)
  {
    log_printf("Failed loading compressed image file \"%s\"!\n", pDst_filename);
    return EXIT_FAILURE;
  }

  log_printf("Compression time: %3.3fms, Decompression time: %3.3fms\n", total_comp_time, total_uncomp_time);
  
  // Write uncompressed image.
  if (output_filename[0])
    stbi_write_tga(output_filename, uncomp_width, uncomp_height, uncomp_req_comps, pUncomp_image_data);

  if ((uncomp_width != width) || (uncomp_height != height))
  {
    log_printf("Loaded JPEG file has a different resolution than the original file!\n");
    return EXIT_FAILURE;
  }

  // Diff the original and compressed images.
  image_compare_results results;
  image_compare(results, width, height, pImage_data, req_comps, pUncomp_image_data, uncomp_req_comps, (params.m_subsampling == jpge::Y_ONLY) || (actual_comps == 1) || (uncomp_actual_comps == 1));
  log_printf("Error Max: %f, Mean: %f, Mean^2: %f, RMSE: %f, PSNR: %f\n", results.max_err, results.mean, results.mean_squared, results.root_mean_squared, results.peak_snr);

  log_printf("Success.\n");

  return EXIT_SUCCESS;
}
