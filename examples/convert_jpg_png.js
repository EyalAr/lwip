/**
 * Example for using LWIP to convert an image from jpg to png.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.writeFile('lena_from_jpg.png', function(err) {
        if (err) return console.log(err);
        console.log('done');
    });
});
