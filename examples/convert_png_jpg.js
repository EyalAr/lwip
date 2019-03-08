/**
 * Example for using LWIP to convert an image from png to jpg.
 */

const lwip = require('../');

lwip.open('lena.png', (err, image) => {
    if (err) return console.log(err);
    image.writeFile('lena_from_png.jpg', err => {
        if (err) return console.log(err);
        console.log('done');
    });
});
