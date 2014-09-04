/**
 * Example for using LWIP to mirror an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .mirror('x')
        .writeFile('lena_mirror.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
