/**
 * Example for using LWIP to crop an image.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .crop(400, 400)
        .writeFile('lena_crop.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
