/**
 * Example for using LWIP to rotate an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .rotate(45, {
            r: 90,
            g: 55,
            b: 40
        })
        .writeFile('lena_rotate.gif', function(err) {
            if (err) return console.log(err);
            console.log('done')
        });
});
