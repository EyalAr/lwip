/**
 * Example for using LWIP to fade an image to 50% transperancy.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .fade(0.5)
        .writeFile('lena_fade.png', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
