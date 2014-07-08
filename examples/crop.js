/**
 * Example for using LWIP to crop an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .crop(400, 400)
        .writeFile('lena_crop.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
