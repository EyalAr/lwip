/**
 * Example for using LWIP to create a highly compressed interlaced png file.
 */

const path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.writeFile('lena_interlaced.png', {
        compression: 'high',
        interlaced: true
    }, err => {
        if (err) return console.log(err);
        console.log('done');
    });
});
