/**
 * Example for using LWIP to contain an image in a canvas.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.contain(400,700,'green',(err, image) => {
        image.writeFile('lena_contain.jpg', err => {
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
