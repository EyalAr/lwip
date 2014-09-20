/**
 * Example for using LWIP to fade an image to 50% transperancy.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.batch()
        .fade(0.5)
        .writeFile('lena_fade.png', function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
