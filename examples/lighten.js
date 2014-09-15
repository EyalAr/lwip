/**
 * Example for using LWIP to increase image lightness by 50%.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .lighten(0.5)
        .writeFile('lena_lighten.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
