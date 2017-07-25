/**
 * Example for using LWIP to shift image's hue by 50 degrees.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.batch()
        .hue(50)
        .writeFile('lena_hue.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
