/**
 * Example for using LWIP to create a highly compressed interlaced png file.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.writeFile('lena_interlaced.png', {
        compression: 'high',
        interlaced: true
    }, function(err) {
        if (err) return console.log(err);
        console.log('done');
    });
});
