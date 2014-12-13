# Development Notes

## Getting started

0. Read README.md to get a sense of the public API and usage conventions.
0. Read the rest of this document (you can skip parts that are not relevant
   to you).
0. Go quickly over the JS code, but don't try to understand each and every line.
0. Try to understand how the JS code uses the native part.
0. __Ask questions.__

## Intro

An image is just a matrix of pixels. Pixels have channels. Grayscale images have
one channel, color images have 3 or more channels (RGB. CMYK, etc.) and
transparent images have an extra alpha channel (RGBA, etc.), which defines how
transparent the respective pixel is.

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

In order to reduce complexity, __we will always represent pixels as 4 channels
RGBA values__. If the image is originally grayscale, we will convert it to RGBA
when opening it. If the image has no transparency, the alpha channel will be
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

The decoder will expose a module with a decoding method for each supported
image format. Each of these methods will decode an image buffer of the
respective format. E.g. `decoder.jpeg( ... )` will decode JPEG buffers.

Each of these functions receives 2 arguments - A Buffer object and a callback
function.

The image Buffer object and the callback function are handed to the decoders
from the JS side. We don't care how the buffer was created (most probably with
`fs.readFile`), as long as it's a valid image buffer of the correct format.

The purpose of the decoder is, well, to decode the image buffer, and generate
a new buffer of raw pixels data. Each decoder function will use the respective
library (libjpeg, libpng, etc.) to do its thing.

The decoding function takes a callback as the second argument. Decoding an image
buffer is done asynchronously. When the decoding is finished, the callback
function is called with __6 parameters__:

0. An Error object, or `null` if no error
0. Raw pixels Buffer object (the decoded image)
0. Width of the image
0. Height of the image
0. Number of channels in the raw pixels buffer. __Currently always 4__ (all
   images are converted to RGBA when decoding. See the `toRGBA` function in
   `util.cpp`).
0. Whether the image has transparency (`true` / `false`).

Calling the callback with these parameters means effectively giving control back
to the JS side.

**Note** that the decoder is not doing _any_ arguments validation. If called
with invalid arguments, __it will crash__. Arguments validation in C++ is a
pain, which is why we never interact with the decoder directly; but rather we
wrap it with JS code which makes sure it's used correctly.

### Encoder

Sources are under `src/encoder`

The encoder, much like the decoder, exposes a module with an encoding method for
each supported image format.

Unlike the decoder, which receives an encoded image buffer; the encoder will
receive a raw pixels buffer to encode. E.g. `encoder.jpeg( ... )` will receive
a raw pixels buffer and encode it into a jpeg image buffer.

The arguments for the encoding methods differ for each encoding format.
We list them here:

0. JPEG: `encoder.jpeg(pixbuff, width, height, quality, callback)`
    0. `pixbuff` - Raw RGBA pixels buffer (a Buffer object)
    0. `width` - The with of the image
    0. `height` - The height of the image
    0. `quality` - JPEG quality (0-100)
    0. `callback` - callback function
0. PNG: `encoder.png(pixbuff, width, height, compression, interlaced, trans, callback)`
    0. `pixbuff` - Raw RGBA pixels buffer (a Buffer object)
    0. `width` - The with of the image
    0. `height` - The height of the image
    0. `compression` - Level of zlib compression:
        - `0` for `Z_NO_COMPRESSION`
        - `1` for `Z_BEST_SPEED`
        - `2` for `Z_BEST_COMPRESSION`
    0. `interlaced` - should the created image be interlaced (`true` / `false`).
    0. `trans` - should the alpha channel be considered when created the PNG?
        - `false`: The alpha channel will be ignored.
        - `true`: The image will be encoded with the alpha channel.
    0. `callback` - callback function

In all cases encoding is done asynchronously. `callback` is a function which
will be called with __2 parameters__:

0. An Error object, or `null` if no error
0. Buffer object of the encoded image

Calling the callback with these parameters means effectively giving control back
to the JS side; in which, for example, the Buffer object can be written to disk
as a file, sent over the network, etc.

**Note** that the encoder is not doing _any_ arguments validation. If called
with invalid arguments, __it will crash__. Arguments validation in C++ is a
pain, which is why we never interact with the encoder directly; but rather we
wrap it with JS code which makes sure it's used correctly.

### Image processor

Sources are under `src/image`

## Structure of the JS part
