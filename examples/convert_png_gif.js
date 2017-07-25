/**
 * Example for using LWIP to convert an image from png to jpg.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.png', (err, image) => {
    if (err) return console.log(err);
    image.writeFile('lena_from_png.gif', err => {
        if (err) return console.log(err);
        console.log('done')
    });
});
