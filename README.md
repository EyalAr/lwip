[![Version](http://img.shields.io/npm/v/lwip.svg)](https://www.npmjs.org/package/lwip)
[![Build Status](https://api.travis-ci.org/EyalAr/lwip.svg?branch=master)](https://travis-ci.org/EyalAr/lwip)
[![Build status](https://ci.appveyor.com/api/projects/status/46mk5218x995svhw/branch/master?svg=true)](https://ci.appveyor.com/project/EyalAr/lwip/branch/master)
[![Coverage Status](https://img.shields.io/coveralls/EyalAr/lwip/master.svg)](https://coveralls.io/r/EyalAr/lwip)

# Light-weight image processor for NodeJS

[![Join the chat at https://gitter.im/EyalAr/lwip](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/EyalAr/lwip?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

0. [Overview](#overview)
  0. [Installation](#installation)
  0. [Usage](#usage)
  0. [Supported formats](#supported-formats)
  0. [Colors specification](#colors-specification)
  0. [Note on transparent images](#note-on-transparent-images)
0. [API](#api)
  0. [Open an image from file or buffer](#open-an-image)
  0. [Create a new blank image](#create-a-new-image)
  0. [Image operations](#image-operations)
    0. [Resize](#resize)
    0. [Scale](#scale)
    0. [Contain](#contain)
    0. [Cover](#cover)
    0. [Rotate](#rotate)
    0. [Crop](#crop)
    0. [Blur](#blur)
    0. [Sharpen](#sharpen)
    0. [Mirror](#mirror)
    0. [Flip](#flip)
    0. [Border](#border)
    0. [Pad](#pad)
    0. [Adjust saturation](#saturate)
    0. Adjust lightness: [lighten](#lighten) / [darken](#darken)
    0. [Adjust hue](#hue)
    0. [Fade (adjust transparency)](#fade)
    0. [Opacify](#opacify)
    0. [Paste](#paste)
    0. [Set pixel](#set-pixel)
    0. [Set metadata](#set-metadata)
  0. [Getters](#getters)
    0. [Width](#width)
    0. [Height](#height)
    0. [Pixel](#get-pixel)
    0. [Clone](#clone)
    0. [Extract / Copy](#extract)
    0. [Get as a Buffer](#get-as-a-buffer)
      0. [JPEG](#jpeg)
      0. [PNG](#png)
      0. [GIF](#gif)
    0. [Write to file](#write-to-file)
    0. [Get metadata](#get-metadata)
  0. [Batch operations](#batch-operations)
0. [Copyrights](#copyrights)

## Overview

This module provides comprehensive, fast, and simple image processing and
manipulation capabilities.

**There are no external runtime dependencies**, which means you don't have to
install anything else on your system.

**This module is in active development. [New features](https://github.com/EyalAr/lwip/issues?labels=feature+request&page=1&state=open) are being added.**

[Read the background for the development of this module.](http://eyalarubas.com/image-processing-nodejs.html)

### Installation

`npm install lwip`

Or, clone this repo and `cd lwip && npm install`.

You can run tests with `npm test`.

**Note:** Installation of this module involves compiling native code.
If `npm install lwip` failes, you probably need to setup your system.
[See instructions](https://github.com/TooTallNate/node-gyp#installation).
Building on Windows with Visual Studio requires version 2013 or higher.

### Usage

**Typical workflow:**

0. Open an image and get an image object.
0. Manipulate it.
0. Save to disk / Send image buffer over network / etc.

**Example (batch operations):**

```Javascript
// obtain an image object:
require('lwip').open('image.jpg', function(err, image){

  // check err...
  // define a batch of manipulations and save to disk as JPEG:
  image.batch()
    .scale(0.75)          // scale to 75%
    .rotate(45, 'white')  // rotate 45degs clockwise (white fill)
    .crop(200, 200)       // crop a 200X200 square from center
    .blur(5)              // Gaussian blur with SD=5
    .writeFile('output.jpg', function(err){
      // check err...
      // done.
    });

});
```

**Example (non-batch):**

```Javascript
var lwip = require('lwip');

// obtain an image object:
lwip.open('image.jpg', function(err, image){

  // check err...
  // manipulate image:
  image.scale(0.5, function(err, image){

    // check err...
    // manipulate some more:
    image.rotate(45, 'white', function(err, image){

      // check err...
      // encode to jpeg and get a buffer object:
      image.toBuffer('jpg', function(err, buffer){

        // check err...
        // save buffer to disk / send over network / etc.

      });

    });

  });

});
```

### Supported formats

**Decoding (reading):**

- JPEG, 1 & 3 channels (grayscale & RGB).
- PNG, transparency supported.
- GIF, transparency supported. Animated GIFs can be read, but only the first
  frame will be retrieved.

**Encoding (writing):**

- JPEG, 3 channels (RGB).
- PNG (lossless), 3 channels (RGB) or 4 channels (RGBA).
- GIF (no animations)

Other formats may also be supported in the future, but are probably less urgent.
Check the issues to see [which formats are planned to be supported](https://github.com/EyalAr/lwip/issues?labels=format+request&page=1&state=open).
Open an issue if you need support for a format which is not already listed.

### Colors specification

In LWIP colors are coded as RGBA values (red, green, blue and an alpha channel).

Colors are specified in one of three ways:

- As a string. possible values:

  ```Javascript
  "black"    // {r: 0, g: 0, b: 0, a: 100}
  "white"    // {r: 255, g: 255, b: 255, a: 100}
  "gray"     // {r: 128, g: 128, b: 128, a: 100}
  "red"      // {r: 255, g: 0, b: 0, a: 100}
  "green"    // {r: 0, g: 255, b: 0, a: 100}
  "blue"     // {r: 0, g: 0, b: 255, a: 100}
  "yellow"   // {r: 255, g: 255, b: 0, a: 100}
  "cyan"     // {r: 0, g: 255, b: 255, a: 100}
  "magenta"  // {r: 255, g: 0, b: 255, a: 100}
  ```

- As an array `[R, G, B, A]` where `R`, `G` and `B` are integers between 0 and
  255 and `A` is an integer between 0 and 100.
- As an object `{r: R, g: G, b: B, a: A}` where `R`, `G` and `B` are integers
  between 0 and 255 and `A` is an integer between 0 and 100.

**Note**: The `A` value (alpha channel) is always optional and defaults to
100 (completely opaque).

### Note on transparent images

0. Transparency is supported through an alpha channel which ranges between 0
   and 100. 0 is completely transparent and 100 is completely opaque.
0. Not all formats support transparency. If an image with an alpha channel is
   encoded with a format which does not support transparency, the alpha channel
   will be ignored (effectively setting it to 100% for all pixels).

## API

All operations are done on an `image` object. An `image` object can be obtained
by:

0. Openning an existing image file or buffer with the [`open`](#open-an-image)
   method.
0. Creating a new image object with the [`create`](#create-a-new-image) method.
0. Cloning an existing image object with the [`image.clone`](#clone) method.
0. Extracting a sub-image from an existing image object with the
   [`image.extract`](#extract) method.

### Open an image

`lwip.open(source, type, callback)`

0. `source {String/Buffer}`: The path to the image on disk or an image buffer.
0. `type {String/Object}`: **Optional** type of the image. If omitted, the type
   will be inferred from the file extension. If `source` is a buffer, `type`
   must be specified. If `source` is an encoded image buffer, `type` must be
   a string of the image type (i.e. `"jpg"`). If `source` is a raw pixels buffer
   `type` must be an object with `type.width` and `type.height` properties.
0. `callback {Function(err, image)}`

**Note about raw pixels buffers:** `source` may be a buffer of raw pixels. The
buffer may contain pixels of 1-4 channels, where:

0. 1 channel is a grayscale image.
0. 2 channels is a grayscale image with an alpha channel.
0. 3 channels is an RGB image.
0. 4 channels is an RGBA image (with an alpha channel).

In other words, if the image in the buffer has width `W` and height `H`, the
size of the buffer can be `W*H`, `2*W*H`, `3*W*H` or `4*W*H`.

The channel values in the buffer must be stored sequentially. I.e. first all the
Red values, then all the Green values, etc.

#### Open file example

```Javascript
var lwip = require('lwip');
lwip.open('path/to/image.jpg', function(err, image){
    // check 'err'. use 'image'.
    // image.resize(...), etc.
});
```

#### Open buffer example

```Javascript
var fs = require('fs'),
    lwip = require('lwip');

fs.readFile('path/to/image.png', function(err, buffer){
  // check err
  lwip.open(buffer, 'png', function(err, image){
      // check 'err'. use 'image'.
      // image.resize(...), etc.
  });
});
```

### Create a new image

`lwip.create(width, height, color, callback)`

0. `width {Integer>0}`: The width of the new image.
0. `height {Integer>0}`: The height of the new image.
0. `color {String / Array / Object}`: **Optional** Color of the canvas. See
   [colors specification](#colors-specification). Defaults to a transparent
   canvas `{r:0, g:0, b:0, a:0}`.
0. `callback {Function(err, image)}`

**Example**:

```Javascript
var lwip = require('lwip');

lwip.create(500, 500, 'yellow', function(err, image){
  // check err
  // 'image' is a 500X500 solid yellow canvas.
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

#### Contain

Contain the image in a colored canvas. The image will be resized to the largest
possible size such that it's fully contained inside the canvas.

`image.contain(width, height, color, inter, callback)`

0. `width {Integer}`: Canvas' width in pixels.
0. `height {Integer}`: Canvas' height in pixels.
0. `color {String / Array / Object}`: **Optional** Color of the canvas. See
   [colors specification](#colors-specification).
0. `inter {String}`: **Optional** interpolation method. Defaults to `"lanczos"`.
   Possible values:
   - `"nearest-neighbor"`
   - `"moving-average"`
   - `"linear"`
   - `"grid"`
   - `"cubic"`
   - `"lanczos"`
0. `callback {Function(err, image)}`

#### Cover

Cover a canvas with the image. The image will be resized to the smallest
possible size such that both its dimensions are bigger than the canvas's
dimensions. Margins of the image exceeding the canvas will be discarded.

`image.cover(width, height, inter, callback)`

0. `width {Integer}`: Canvas' width in pixels.
0. `height {Integer}`: Canvas' height in pixels.
0. `inter {String}`: **Optional** interpolation method. Defaults to `"lanczos"`.
   Possible values:
   - `"nearest-neighbor"`
   - `"moving-average"`
   - `"linear"`
   - `"grid"`
   - `"cubic"`
   - `"lanczos"`
0. `callback {Function(err, image)}`

#### Rotate

`image.rotate(degs, color, callback)`

0. `degs {Float}`: Clockwise rotation degrees.
0. `color {String / Array / Object}`: **Optional** Color of the canvas. See
   [colors specification](#colors-specification).
0. `callback {Function(err, image)}`

#### Crop

#### Crop with rectangle coordinates

`image.crop(left, top, right, bottom, callback)`

0. `left, top, right, bottom {Integer}`: Coordinates of the crop rectangle.
0. `callback {Function(err, image)}`

#### Crop a rectangle from center

`image.crop(width, height, callback)`

0. `width, height {Integer}`: Width and height of the rectangle to crop from the
   center of the image.
0. `callback {Function(err, image)}`

#### Blur

Gaussian blur.

`image.blur(sigma, callback)`

0. `sigma {Float>=0}`: Standard deviation of the Gaussian filter.
0. `callback {Function(err, image)}`

#### Sharpen

Inverse diffusion shapren.

`image.sharpen(amplitude, callback)`

0. `amplitude {Float}`: Sharpening amplitude.
0. `callback {Function(err, image)}`

#### Mirror

Mirror an image along the 'x' axis, 'y' axis or both.

`image.mirror(axes, callback)`

0. `axes {String}`: `'x'`, `'y'` or `'xy'` (case sensitive).
0. `callback {Function(err, image)}`

#### Flip

Alias of [`mirror`](#mirror).

#### Border

Add a colored border to the image.

`image.border(width, color, callback)`

0. `width {Integer}`: Border width in pixels.
0. `color {String / Array / Object}`: **Optional** Color of the border. See
   [colors specification](#colors-specification).
0. `callback {Function(err, image)}`

#### Pad

Pad image edges with colored pixels.

`image.pad(left, top, right, bottom, color, callback)`

0. `left, top, right, bottom {Integer}`: Number of pixels to add to each edge.
0. `color {String / Array / Object}`: **Optional** Color of the padding. See
   [colors specification](#colors-specification).
0. `callback {Function(err, image)}`

#### Saturate

Adjust image saturation.

`image.saturate(delta, callback)`

0. `delta {Float}`: By how much to increase / decrease the saturation.
0. `callback {Function(err, image)}`

**Examples**:

0. `image.saturate(0, ...)` will have no effect on the image.
0. `image.saturate(0.5, ...)` will increase the saturation by 50%.
0. `image.saturate(-1, ...)` will decrease the saturation by 100%, effectively
   desaturating the image.

#### Lighten

Adjust image lightness.

`image.lighten(delta, callback)`

0. `delta {Float}`: By how much to increase / decrease the lightness.
0. `callback {Function(err, image)}`

**Examples**:

0. `image.lighten(0, ...)` will have no effect on the image.
0. `image.lighten(0.5, ...)` will increase the lightness by 50%.
0. `image.lighten(-1, ...)` will decrease the lightness by 100%, effectively
   making the image black.

#### Darken

Adjust image lightness.

`image.darken(delta, callback)`

Equivalent to `image.lighten(-delta, callback)`.

#### Hue

Adjust image hue.

`image.hue(shift, callback)`

0. `shift {Float}`: By how many degrees to shift each pixel's hue.
0. `callback {Function(err, image)}`

**Examples**:

0. `image.lighten(0, ...)` will have no effect on the image.
0. `image.lighten(100, ...)` will shift pixels' hue by 100 degrees.

**Note:** The hue is shifted in a circular manner in the range [0,360] for each
pixel individually.

#### Fade

Adjust image transperancy.

`image.fade(delta, callback)`

0. `delta {Float}`: By how much to increase / decrease the transperancy.
0. `callback {Function(err, image)}`

**Note:** The transparency is adjusted independently for each pixel.

**Examples**:

0. `image.fade(0, ...)` will have no effect on the image.
0. `image.fade(0.5, ...)` will increase the transparency by 50%.
0. `image.fade(1, ...)` will make the image completely transparent.

#### Opacify

Make image completely opaque.

`image.opacify(callback)`

0. `callback {Function(err, image)}`

#### Paste

Paste an image on top of this image.

`image.paste(left, top, img, callback)`

0. `left, top {Integer}`: Coordinates of the top-left corner of the pasted
   image.
0. `img {Image object}`: The image to paste.
0. `callback {Function(err, image)}`

**Notes:**

0. If the pasted image exceeds the bounds of the base image, an exception
   is thrown.
0. `img` is pasted in the state it was at the time `image.paste( ... )` was
   called, eventhough `callback` is called asynchronously.
0. For transparent images, alpha blending is done according to the equations
   described [here](http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending).
0. Extra caution is required when using this method in batch mode, as the images
   may change by the time this operation is called.

#### Set Pixel

Set the color of a pixel.

`image.setPixel(left, top, color, callback)`

0. `left, top {Integer}`: Coordinates of the pixel from the left-top corner of
   the image.
0. `color {String / Array / Object}`: Color of the pixel to set.
   See [colors specification](#colors-specification).
0. `callback {Function(err, image)}`

**Notes:**

0. If the coordinates exceed the bounds of the image, an exception is thrown.
0. Extra caution is required when using this method in batch mode, as the
  dimensions of the image may change by the time this operation is called.

#### Set metadata

Set the metadata in an image. This is currently only supported for PNG files.
Sets a tEXt chunk with the key `lwip_data` and comment as the given string. If
called with a `null` parameter, removes existing metadata from the image,
if present.

`image.setMetadata(metadata)`

0. `metadata {String}`: a string of arbitrary length, or null.

### Getters

#### Width

`image.width()` returns the image's width in pixels.

#### Height

`image.height()` returns the image's height in pixels.

#### Get Pixel

`image.getPixel(left, top)` returns the color of the pixel at the `(left, top)`
coordinate.

0. `left {Integer>=0}`
0. `top {Integer>=0}`

Color is returned as an object. See [colors specification](#colors-specification).

#### Clone

Clone the image into a new image object.

`image.clone(callback)`

0. `callback {Function(err, newImage)}`

**Example**: See [`examples/clone.js`](examples/clone.js)

**Note**: The image is cloned to the state it was at the time
`image.clone( ... )` was called, eventhough `callback` is called asynchronously.

```Javascript
image.width(); // 500
image.clone(function(err, clone){
    clone.width(); // 500
});
image.resize(100, 100, function(err, image){
    image.width(); //100
});
```

#### Extract

Copy an area of the image into a new image object.

`image.extract(left, top, right, bottom, callback)`

0. `left, top, right, bottom {Integer}`: Coordinates of the area to copy.
0. `callback {Function(err, newImage)}`

**Example**: See [`examples/extract.js`](examples/extract.js)

**Note**: The sub-image is extracted from the original image in the state it was
at the time `image.extract( ... )` was called, eventhough `callback` is called
asynchronously.

#### Get as a Buffer

Get encoded binary image data as a NodeJS
[Buffer](http://nodejs.org/api/buffer.html).

When opening an image, it is decoded and stored in memory as an uncompressed
image. All manipulations are done on the uncompressed data in memory. This
method allows to encode the image to one of the specified formats and get the
encoded data as a NodeJS Buffer object.

`image.toBuffer(format, params, callback)`

0. `format {String}`: Encoding format. Possible values:
  - `"jpg"`
  - `"png"`
  - `"gif"`
0. `params {Object}`: **Optional** Format-specific parameters (See below).
0. `callback {Function(err, buffer)}`

**Supported encoding formats:**

##### JPEG

The `params` object should have the following fields:

- `quality {Integer}`: Defaults to `100`.

Note that when encoding to JPEG the alpha channel is discarded.

##### PNG

The `params` object should have the following fields:

- `compression {String}`: Defaults to `"fast"`. Possible values:
  - `"none"` - No compression. Fastest.
  - `"fast"` - Basic compression. Fast.
  - `"high"` - High compression. Slowest.
- `interlaced {Boolean}`: Defaults to `false`.
- `transparency {true/false/'auto'}`: Preserve transparency? Defaults to
  `'auto'`. Determines if the encoded image will have 3 or 4 channels. If
  `'auto'`, the image will be encoded with 4 channels if it has transparent
  components, and 3 channels otherwise.

##### GIF

The `params` object should have the following fields:

- `colors {Integer}`: Defaults to `256`. Number of colors in the color table
  (at most). Must be between 2 and 256.
- `interlaced {Boolean}`: Defaults to `false`.
- `transparency {true/false/'auto'}`: Preserve transparency? Defaults to
  `'auto'`. Determines if the encoded image will have 3 or 4 channels. If
  `'auto'`, the image will be encoded with 4 channels if it has transparent
  components, and 3 channels otherwise.
- `threshold {Integer}` - Between 0 and 100. Pixels in a gif image are either
  fully transparent or fully opaque. This value sets the alpha channel
  threshold to determine if a pixel is opaque or transparent. If the alpha
  channel of the pixel is above this threshold, this pixel will be considered
  as opaque; otherwise it will be transparent.

#### Write to file

Write encoded binary image data directly to a file.

`image.writeFile(path, format, params, callback)`

0. `path {String}`: Path of file to write.
0. `format {String}`: **Optional** Encoding format. If omitted, will be inferred
   from `path` extension. Possible values are specified in
   [Get as a Buffer](#get-as-a-buffer) section.
0. `params {Object}`: **Optional** Format-specific parameters.
0. `callback {Function(err)}`

#### Get Metadata

Get the textual metadata from an image. This is currently only supported for
tEXt chunks in PNG images, and will get the first tEXt chunk found with the key
`lwip_data`. If none is found, returns null.

`image.getMetadata()`

### Batch operations

Each of the [image operations](#image-operations) above can be done as part of
a batch of operations. Operations can be queued, and executed as a batch at any
time.

Each one of the [image operations](#image-operations) has a batch equivalent
which takes the same arguments, except the callback, which is not needed.

When all batch operations had been queued, they can be executed in one of
several methods, as explained below.

#### Obtaining a batch object

In order to start queueing operations, a batch object first needs to be obtained
from the image.

```Javascript
// obtain a batch object from the image:
var batch = image.batch();
```

#### Using a batch object

Use the batch object to queue [image operations](#image-operations). Each of the
operations above has a batch equivalent. Operations can be chained.

**Remember, the batch manipulation methods do not take a callback.**

**Example:**

```Javascript
batch.rotate(45, 'white').scale(0.5).blur(5);
```

#### Executing a batch

There are several methods which start the execution of a batch. Once a batch
finishes an execution, it becomes empty and can be resued to queue additional
operations.

##### Execute batch and obtain the manipulated image object

When all desired operations had been queued, execute the batch with the `exec()`
method. `exec` takes a `callback` argument; `callback` is a function which
receives an error object and the manipulated image object:

`batch.exec(callback)`

  - `callback {Function(err, image)}`:
    - `err`: An error object or `null` when no error.
    - `image`: An image object of the manipulated image.

```Javascript
batch.exec(function(err, image){
  // check err, use image
});
```

##### Execute batch and obtain a Buffer object

Batch objects have a `toBuffer` convenience method.

`batch.toBuffer(format, params, callback)`

See parameters of [`image.toBuffer()`](#get-as-a-buffer).

##### Execute batch and write to file

Batch objects have a `writeFile` convenience method.

`batch.writeFile(path, format, params, callback)`

See parameters of [`image.writeFile()`](#write-to-file).

#### Notes on batch operations

An image can have more than one batch object, but all batch objects modify the
same underlying image. This means the order of execution matters.

```Javascript
var batch1 = image.batch().rotate('45', 'black');
var batch2 = image.batch().border(15, 'black');
```

This will rotate the image 45degs and then add a black border:

```Javascript
batch1.exec(function(err, image){
    batch2.exec(function(err, image){
        // ...
    });
});
```

While this will add a black border and then rotate the image 45degs:

```Javascript
batch2.exec(function(err, image){
    batch1.exec(function(err, image){
        // ...
    });
});
```

## Copyrights

The native part of this module is compiled from source which uses the following:

- Independent JPEG Group's free JPEG software:
  - [Website](http://www.ijg.org/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/src/lib/jpeg/README)
- libpng:
  - [Website](http://www.libpng.org/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/src/lib/png/README)
- zlib:
  - [Website](http://www.zlib.net/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/src/lib/zlib/README)
- The CImg Library
  - [Website](http://cimg.sourceforge.net/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/src/lib/cimg/README.txt)
- giflib
  - [Website](http://giflib.sourceforge.net/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/src/lib/gif/README)
