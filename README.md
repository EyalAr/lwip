# Light-weight image processor for NodeJS

0. [Overview](#overview)
0. [API](#api)
  0. [Open an image](#open-an-image)
  0. [Image operations](#image-operations)
    0. [Resize](#resize)
    0. [Scale](#scale)
  0. [Getters](#getters)
    0. [Width](#width)
    0. [Height](#height)
    0. [Get as a Buffer](#get-as-a-buffer)
      0. [JPEG](#jpeg)
0. [Benchmarks](#benchmarks)
0. [Copyrights](#copyrights)

## Overview

This module provides comprehensive, fast, and simple image processing and
manipulation capabilities.

**There are no external runtime dependencies**, which means you don't have to
install anything else on your system. Just ~~`npm install lwip`~~ (coming soon)
and you're ready.

**This module is in active development. New features are being added.**

### Typical workflow:

0. Open an image and get an image object.
0. Manipulate it.
0. Get Buffer object of the image encoded as some format (JPEG for example).
0. Save to disk / Send over network / etc.

## API

All operations are done on an `image` object. An `image` object is obtained with
the `open` method.

### Open an image

`open(path, type, callback)`

0. `path {String}`: The path to the image on disk.
0. `type {String}`: **Optional** type of the image. If omitted, the type will be
   inferred from the file extension. Can usually be omitted. Useful to open
   image files without extensions.
0. `callback {Function(err, image)}`

```Javascript
var lwip = require('lwip');
lwip.open('path/to/image.jpg', function(err, image){
    // check 'err'. use 'image'.
    // image.resize(...), etc.
});
```

### Image operations

#### Resize

`image.resize(width, height, inter, callback)`

0. `width {Integer}`: Width in pixels.
0. `height {Integer}`: **Optional** height in pixels. If omitted, `width` will
   be used.
0. `inter {String}`: **Optional** interpolation method. Defaults to `"lanczos"`.
   Possible values:
   - `"nearest-neighbor"`
   - `"moving-average"`
   - `"linear"`
   - `"grid"`
   - `"cubic"`
   - `"lanczos"`
0. `callback {Function(err, image)}`

#### Scale

`image.scale(wRatio, hRatio, inter, callback)`

0. `wRatio {Float}`: Width scale ratio.
0. `hRatio {Float}`: **Optional** height scale ratio. If omitted, `wRatio` will
   be used.
0. `inter {String}`: **Optional** interpolation method. Defaults to `"lanczos"`.
   Possible values:
   - `"nearest-neighbor"`
   - `"moving-average"`
   - `"linear"`
   - `"grid"`
   - `"cubic"`
   - `"lanczos"`
0. `callback {Function(err, image)}`

### Getters

#### Width

`image.width()` returns the image's width in pixels.

#### Height

`image.height()` returns the image's height in pixels.

#### Get as a Buffer

Get encoded binary image data as a NodeJS
[Buffer](http://nodejs.org/api/buffer.html).

When opening an image, it is decoded and stored in memory as an uncompressed
image. All manipulations are done on the uncompressed data in memory. This
method allows to encode the image to one of the specified formats and get the
encoded data as a NodeJS Buffer object.

`image.toBuffer(format, params, callback)`

0. `format {String}`: Encoding format. Possible values:
  -. `"jpg"`
0. `params {Object}`: Format-specific parameters (See below).
0. `callback {Function(err, buffer)}`

**Supported encoding formats:**

##### JPEG

The `params` object should have the following fields:

- `quality {Integer}`

## Copyrights

The native part of this module is compiled from source which uses the following:

- Independent JPEG Group's free JPEG software:
  - [Website](http://www.ijg.org/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/lib/jpeg/README)
- The CImg Library
  - [Website](http://cimg.sourceforge.net/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/lib/cimg/README.txt)
