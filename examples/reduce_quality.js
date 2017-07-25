/**
 * Example for using LWIP to reduce the quality of a JPEG file.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.writeFile('lena_low_quality.jpg', {
        quality: 10
    }, err => {
        if (err) return console.log(err);
        console.log('done');
    });
});
