/**
 * Example for using LWIP to mirror an image.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .mirror('x')
        .writeFile('lena_mirror.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
