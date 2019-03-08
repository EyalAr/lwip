/**
 * Example for using LWIP to convert an image from png to jpg.
 */

const lwip = require('../');

lwip.open('lena.gif', (err, image) => {
    if (err) return console.log(err);
    image.writeFile('lena_from_gif.png', err => {
        if (err) return console.log(err);
        console.log('done');
    });
});
