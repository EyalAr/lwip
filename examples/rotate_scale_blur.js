/**
 * Example for using LWIP for several batch operations.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.png', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .rotate(-33, 'white')
        .scale(1.5)
        .blur(5)
        .writeFile('lena_rotate_scale_blur.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
