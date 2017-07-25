/**
 * Example for using LWIP to sharpen an image.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .sharpen(200)
        .writeFile('lena_sharpen.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
