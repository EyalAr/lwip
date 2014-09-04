[![Build Status](https://api.travis-ci.org/EyalAr/lwip.svg?branch=master)](https://travis-ci.org/EyalAr/lwip)
[![Stories in Ready](https://badge.waffle.io/eyalar/lwip.png?label=ready&title=Ready)](https://waffle.io/eyalar/lwip)
[![Stories in Ready](https://badge.waffle.io/eyalar/lwip.png?label=in+progress&title=In+Progress)](https://waffle.io/eyalar/lwip)

# Light-weight image processor for NodeJS

0. [Overview](#overview)
  0. [Installation](#installation)
  0. [Usage](#usage)
  0. [Supported formats](#supported-formats)
0. [API](#api)
  0. [Open an image](#open-an-image)
  0. [Image operations](#image-operations)
    0. [Resize](#resize)
    0. [Scale](#scale)
    0. [Rotate](#rotate)
    0. [Crop](#crop)
    0. [Blur](#blur)
    0. [Mirror](#mirror)
    0. [Flip](#flip)
    0. [Border](#border)
    0. [Pad](#pad)
  0. [Getters](#getters)
    0. [Width](#width)
    0. [Height](#height)
    0. [Get as a Buffer](#get-as-a-buffer)
      0. [JPEG](#jpeg)
      0. [PNG](#png)
    0. [Write to file](#write-to-file)
  0. [Batch operations](#batch-operations)
0. [Copyrights](#copyrights)

## Overview

This module provides comprehensive, fast, and simple image processing and
manipulation capabilities.

**There are no external runtime dependencies**, which means you don't have to
install anything else on your system.

**This module is in active development. [New features](https://github.com/EyalAr/lwip/issues?labels=feature+request&page=1&state=open) are being added.**

### Installation

`npm install lwip`

Or, clone this repo and `cd lwip && npm install`.

You can run tests with `npm test`.

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
    .crop(200)            // crop a 200X200 square from center
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
- PNG, 1 & 3 channels (grayscale & RGB). Alpha channel (transperancy) is not
  currently supported.

**Encoding (writing):**

- JPEG, 3 channels (RGB).
- PNG (lossless), 3 channels (RGB).

Other formats may also be supported in the future, but are probably less urgent.
Check the issues to see [which formats are planned to be supported](https://github.com/EyalAr/lwip/issues?labels=format+request&page=1&state=open).
Open an issue if you need support for a format which is not already listed.

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

#### Rotate

`image.rotate(degs, color, callback)`

0. `degs {Float}`: Clockwise rotation degrees.
0. `color {String / Array / Object}`: **Optional** Color of the canvas.
  - As a string, possible values: `"black"`, `"white"`, `"gray"`, `"blue"`,
    `"red"`, `"green"`, `"yellow"`, `"cyan"`, `"magenta"`.
  - As an array `[R, G, B]` where `R`, `G` and `B` are integers between 0 and
    255.
  - As an object `{r: R, g: G, b: B}` where `R`, `G` and `B` are integers
    between 0 and 255.
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

0. `sigma {Float}`: Standard deviation of the Gaussian filter.
0. `callback {Function(err, image)}`

#### Mirror

Mirror an image along the 'x' axis, 'y' axis or both.

`image.mirror(axes, callback)`

0. `axes {String}`: `'x'`, `'y'` or `'xy'`.
0. `callback {Function(err, image)}`

#### Flip

Alias of [`mirror`](#mirror).

#### Border

Add a colored border to the image.

`image.border(width, color, callback)`

0. `width {Integer}`: Border width in pixels.
0. `color {String / Array / Object}`: **Optional** Color of the border.
  - As a string, possible values: `"black"`, `"white"`, `"gray"`, `"blue"`,
    `"red"`, `"green"`, `"yellow"`, `"cyan"`, `"magenta"`.
  - As an array `[R, G, B]` where `R`, `G` and `B` are integers between 0 and
    255.
  - As an object `{r: R, g: G, b: B}` where `R`, `G` and `B` are integers
    between 0 and 255.
0. `callback {Function(err, image)}`

#### Pad

Pad image edges with colored pixels.

`image.pad(left, top, right, bottom, color, callback)`

0. `left, top, right, bottom {Integer}`: Number of pixels to add to each edge.
0. `color {String / Array / Object}`: **Optional** Color of the padding.
  - As a string, possible values: `"black"`, `"white"`, `"gray"`, `"blue"`,
    `"red"`, `"green"`, `"yellow"`, `"cyan"`, `"magenta"`.
  - As an array `[R, G, B]` where `R`, `G` and `B` are integers between 0 and
    255.
  - As an object `{r: R, g: G, b: B}` where `R`, `G` and `B` are integers
    between 0 and 255.
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
  - `"jpg"`
  - `"png"`
0. `params {Object}`: **Optional** Format-specific parameters (See below).
0. `callback {Function(err, buffer)}`

**Supported encoding formats:**

##### JPEG

The `params` object should have the following fields:

- `quality {Integer}`: Defaults to `100`.

##### PNG

The `params` object should have the following fields:

- `compression {String}`: Defaults to `"fast"`. Possible values:
  - `"none"` - No compression. Fastest.
  - `"fast"` - Basic compression. Fast.
  - `"high"` - High compression. Slowest.
- `interlaced {Boolean}`: Defaults to `false`.

#### Write to file

Write encoded binary image data directly to a file.

`image.writeFile(path, format, params, callback)`

0. `path {String}`: Path of file to write.
0. `format {String}`: **Optional** Encoding format. If omitted, will be inferred
   from `path` extension. Possible values are specified in
   [Get as a Buffer](#get-as-a-buffer) section.
0. `params {Object}`: **Optional** Format-specific parameters.
0. `callback {Function(err)}`

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
  - [Readme](https://github.com/EyalAr/lwip/blob/master/lib/jpeg/README)
- libpng:
  - [Website](http://www.libpng.org/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/lib/png/README)
- zlib:
  - [Website](http://www.zlib.net/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/lib/zlib/README)
- The CImg Library
  - [Website](http://cimg.sourceforge.net/)
  - [Readme](https://github.com/EyalAr/lwip/blob/master/lib/cimg/README.txt)
