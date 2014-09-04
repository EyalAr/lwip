/**
 * Example for using LWIP to add a border to an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .border(10, 'green')
        .writeFile('lena_border.jpg', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
