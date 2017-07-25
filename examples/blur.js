/**
 * Example for using LWIP to blur an image.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .blur(10)
        .writeFile('lena_blur.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
