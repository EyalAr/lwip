/**
 * Example for using LWIP to create an interlaced gif file.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.writeFile('lena_interlaced.gif', {
        colors: 222,
        interlaced: true
    }, err => {
        if (err) return console.log(err);
        console.log('done');
    });
});
