/**
 * Example for using LWIP to sharpen an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .sharpen(200)
        .writeFile('lena_sharpen.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
