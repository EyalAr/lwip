/**
 * Example for using LWIP to increase image saturation.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .saturate(0.5)
        .writeFile('lena_saturate.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
