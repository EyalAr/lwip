/**
 * Example for using LWIP to manually setting pixels in an image.
 *
 * Creates a canvas with a rainbow gradient.
 */

var path = require('path'),
    lwip = require('../');

var output = 'rainbow.png',
    width = 360,
    height = 360,
    color = {h: 0, s: 100, l: 50};

function nextColor(){
    color.h++;
    if (color.h > 360) color.h = 0;
    return hslToRgb(color);
}

// create an empty image canvas
lwip.create(width, height, function(err, image){
    if (err) return console.log(err);

    var x, y, c,
        batch = image.batch();

    // set the same color for each columns in the image
    for (x = 0; x < width ; x++){
        c = nextColor();
        for (y = 0; y < height; y++){
            batch.setPixel(x, y, c);
        }
    }

    batch.writeFile(output, function(err){
        if (err) console.log(err);
    });
});

// adapted from http://stackoverflow.com/a/9493060/1365324
function hslToRgb(hsl){
    var h = hsl.h / 360,
        s = hsl.s / 100,
        l = hsl.l / 100;

    var r, g, b;

    if(s == 0){
        r = g = b = l; // achromatic
    }else{
        function hue2rgb(p, q, t){
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(t < 1/6) return p + (q - p) * 6 * t;
            if(t < 1/2) return q;
            if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
            return p;
        }

        var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        var p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }

    return {
        r: Math.round(r * 255),
        g: Math.round(g * 255),
        b: Math.round(b * 255)
    };
}
