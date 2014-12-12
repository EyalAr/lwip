# Development Notes

## Intro

An image is just a matrix of pixels. Pixels have channels. Grayscale images have
one channel, color images have 3 or more channels (RGB. CMYK, etc.) and
transparent images have 4 or more channels (RGBA, etc.), one of which we call
an "alpha" channel; which defines how transparent the respective pixel is.

Pixel color can be represented in various ways. It can be broken down to base
color components such as in RGB and CMYK. It can be encoded as an indexed color
palette, where the palette itself can be RGB or CMYK.

Storing the image's pixels as they are is wasteful. There are methods to encode
and compress the pixels such that the image takes less space. Some of those
methods loose data in the process of compression ("lossy compressions"), and
thus the quality of the image is reduced. Some methods don't loose data in the
compressions process ("lossless compression"), and thus the quality of the image
is preserved. We call those methods "image formats".

For example:

- JPEG does lossy compression (first it discards image data, then it compresses
  it).
- PNG does lossless compression (just compresses the data, without loosing any
  of it).

Different formats support different color representations and compressions
methods.

## Opening an image

Images exist in the wild in various formats with various color representations.
Before working on an image we need to obtain the decoded and uncompressed pixels
data. Each format has its own methods of decoding and decompression.

By "opening an image", we mean - going through the process of having the pixels
in memory. Once we have that, we can start manipulating the image.

We need to decide how to represent uncompressed pixels data in memory.

- What if we open a grayscale image?
- What if we open a 3 channels colored image?
- What if we open a 4 channels colored image?
- What if we open an image with an alpha channel?

In order to reduce complexity, we will always represent pixels as 4 channels
RGBA values. If the image is originally grayscale, we will convert it to RGB
when opening it. If the image has no transparency, the alpha channels will be
100% for all pixels.

In order to decode an image, we have to identify its format. Once we do that,
we can use the appropriate decoding library.

Images are constructed as `CImg<unsigned char>`. Notice the type of a pixel
channel is `unsigned char`, which makes our images 8bit.

Once an image is opened, the underlying raw pixels buffer will always have a
size of `4*W*H` bytes (4 channels, each with `W*H`) pixels, where `W` and `H`
are the width and height, respectively. The channels in the buffer are ordered
sequentially. I.e. first all values of the Red channel, then the Green, etc.

## Structure of the native part

Basically, there's nothing inherent in image handling that says it can't be
done in pure JavaScript. So why have a native part?

0. Existing de facto official image decoding / encoding libraries are written
   in C/C++ (libjpeg, libpng, giflib, etc.).
0. Implementing image encoding / decoding libraries (such as JPEG) in JS is
   _a lot_ of work.
0. ...and includes lots of memory management and handling, which may not be
   efficient in JS.
0. Image processing algorithms, though obviously can be implemented in JS, are
   much faster in native code with direct memory access.

Disclaimer: I would love to get rid of the native part in favor of pure JS
implementations. The day comparable image encoding / decoding libraries will be
available in JS, I'll seriously consider switching.

There are three native parts to lwip, each a standalone module:

### Decoder

Sources are under `src/decoder`

The decoder will expose a JS module with a decoding method for each supported
image format. Each of these methods will decode an image buffer of the
respective format. E.g. `decoder.jpeg( ... )` will decode JPEG buffers.

Each of these functions receive 2 arguments - A Buffer object and a callback
function.

The image Buffer object and the callback function are handed to the decoders
from the JS side. We don't care how the buffer was created (most probably with
`fs.readFile`), as long as it's a valid image buffer of the correct format.

The purpose of the decoder is, well, to decode the image buffer, and generate
a new buffer of raw pixels data. Each decoder function will use the respective
library (libjpeg, libpng, etc.) to do its thing.

The decoding function takes a callback as the second argument. Decoding an image
buffer is done asynchronously. When the decoding is finished, the callback
function is called with __6 arguments__:

0. An error, or `null` if no error
0. Raw pixels buffer (the decoded image)
0. Width of the image
0. Height of the image
0. Number of channels in the raw pixels buffer. Currently always 4 (all images
   are converted to RGBA when decoding).
0. Whether the image has transparency (`true` / `false`).

### Encoder

Sources are under `src/encoder`

### Image processor

Sources are under `src/image`

## Structure of the JS part
