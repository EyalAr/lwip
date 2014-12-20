/**
 * Example for using LWIP to convert an image from png to jpg.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.png', function(err, image) {
    if (err) return console.log(err);
    image.writeFile('lena_from_png.gif', function(err) {
        if (err) return console.log(err);
        console.log('done')
    });
});
