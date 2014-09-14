/**
 * Example for using LWIP to saturate an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .hslAdjust(2,10,0.2)
        .writeFile('lena_hsl.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
