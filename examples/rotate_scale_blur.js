/**
 * Example for using LWIP for several batch operations.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.png', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .rotate(-33, 'white')
        .scale(1.5)
        .blur(5)
        .writeFile('lena_rotate_scale_blur.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
