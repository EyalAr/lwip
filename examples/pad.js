/**
 * Example for using LWIP to pad an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .pad(10, 5, 10, 5, 'blue')
        .writeFile('lena_pad.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
