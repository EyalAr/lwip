/**
 * Example for using LWIP to cover a canvas with an image.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.cover(400,800,(err, image) => {
        image.writeFile('lena_cover.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
