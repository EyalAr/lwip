/**
 * Example for using LWIP to pad an image.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .pad(10, 5, 10, 5, 'blue')
        .writeFile('lena_pad.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
