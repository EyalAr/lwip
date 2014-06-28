# Light-weight image processor for NodeJS

## Overview

This is a native NodeJS module which allows minimal, fast, image processing.
**Without any external runtime dependencies**.

### Image operations

0. Resize
0. Rotate
0. Crop
0. Get image data as NodeJS Buffer object.

### Typical workflow:

0. Load an image
0. Manipulate it
0. Get Buffer object
0. Save to disk / Send over network / etc.

## API

All operations are done on an `image` object. An `image` object is obtained with
the `open` method.

Note that `open` is the only async operation, as it's the only IO operation.
All other operations are done in-memory.

### Open an image

`open(path, callback)`

0. `path`: The path to the image on disk.
0. `callback`: A callback function with two arguments: `callback(err, image)`.

```Javascript
var lwip = require('lwip');
lwip.open('path/to/image', function(err, image){
    // use 'image'
});
```

### Image operations

**Note**: All the following (non-getter) operations

0. Modify the image object on which they are called.
0. Return the image object; thus they are chainable.
   Example: `image.resize(100,100).rotate(90);`.

#### Resize

`image.resize(width, height)`

0. `width {unsigned int}`
0. `height {unsigned int}`

`width` and `height` are specified in pixels. If either one is
`null` / `undefined` / negative, the other will be used to resize while keeping
the existing aspect ratio. If both are `null` / `undefined` / negative, this is
a noop.

Note that scaling down an image is not reversible. You can resize it again to
a larger size, but it will result in quality degradation. To revert a
scale-down you will need to re-open the image.

#### Rotate

`image.rotate(degs, color)`

0. `degs {float}`: The amount of degrees to rotate by, relative to the
   horizontal positive axis, counter-clockwise. Defaults to `0`.
0. `color {Array[int, int, int]}`: The RGB code (as an array of three integers)
   to be used as the background color. If `null` or otherwise invalid, the
   background will be white.

**Note:** Rotating the image will enlarge its dimensions to accommodate for the
increase in width and height. Use `crop` with the old dimensions if you want to
keep the original width and height.

#### Crop

`image.crop(x1 ,y1 [,x2 ,y2])`

`x1`, `y1`, `x2`, `y2` define the crop rectangle. Negative / out of bounds
values will be defaulted as follows:

`x1` and `y1` default to `0`. `x2` and `y2` default to the image's width and
height, respectively.

### Getters

#### Width

`image.width()` returns the image's width in pixels.

#### Height

`image.height()` returns the image's height in pixels.

#### Get as a Buffer

`image.toBuffer()` will return a NodeJS
[Buffer](http://nodejs.org/api/buffer.html) with the binary content of the
image.
