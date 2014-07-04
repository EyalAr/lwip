/**
 * Example for using LWIP to reduce the quality of a JPEG file.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.writeFile('lena_low_quality.jpg', {
        quality: 10
    }, function(err) {
        if (err) return console.log(err);
        console.log('done');
    });
});
