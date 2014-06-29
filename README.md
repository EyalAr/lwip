# Light-weight image processor for NodeJS

## Overview

This is a native NodeJS module which allows minimal, fast, image processing.
**Without any external runtime dependencies**.

### Image operations

0. Resize
0. Rotate
0. Crop
0. Get binary image data as NodeJS Buffer object.

### Typical workflow:

0. Load an image
0. Manipulate it
0. Get Buffer object
0. Save to disk / Send over network / etc.

## API

All operations are done on an `image` object. An `image` object is obtained with
the `open` method.

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

#### Resize

`image.resize(width, height, callback)`

0. `width {unsigned int}`
0. `height {unsigned int}`
0. `callback {Function(err, image)}`

`width` and `height` are specified in pixels. If either one is not specified,
the other one will be used to resize while keeping the existing aspect ratio.
If both are not specified the image will not be changed.

Note that scaling down an image is not reversible. You can resize it again to
a larger size, but it will result in quality degradation. To revert a
scale-down you will need to re-open the image.

#### Rotate

`image.rotate(degs, color, callback)`

0. `degs {float}`: The amount of degrees to rotate by, relative to the
   horizontal positive axis, counter-clockwise.
0. `color {Array[int, int, int]}`: The RGB code (as an array of three integers)
   to be used as the canvas color. If invalid or not specified, the canvas will
   be white.
0. `callback {Function(err, image)}`

**Note:** Rotating the image will enlarge its dimensions to accommodate for the
increase in width and height. Use `crop` with the old dimensions if you want to
keep the original width and height.

#### Crop / Pad

`image.crop(x1 ,y1 ,x2 ,y2, color, callback)`  
**Alias** `image.pad(...)`

`x1`, `y1`, `x2`, `y2` define the crop / pad rectangle. `(x1,y1)` is the
top-left corner and `(x2,y2)` is the bottom-right corner.

If `x1` or `y1` are negative the canvas will be padded to the left and top of
the image, respectively. If `x2` or `y2` are larger than the image's dimensions
the canvas will be padded to the right and bottom of the image, respectively.

0. `color {Array[int, int, int]}`: The RGB color code (as an array of three
   integers) of the canvas when padding. If invalid or not specified, the canvas
   will be white.
0. `callback {Function(err, image)}`

### Getters

#### Width

`image.width()` returns the image's width in pixels.

#### Height

`image.height()` returns the image's height in pixels.

#### Get as a Buffer

Get binary image data as a NodeJS [Buffer](http://nodejs.org/api/buffer.html).

`image.toBuffer(format, callback)`

0. `format{String}`: **Supported formats**:
  0. `"raw"`: The uncompressed image's pixels data.
  0. `"jpg"` / `"jpeg"`
0. `callback {Function(err, buffer)}`
