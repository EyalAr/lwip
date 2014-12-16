/**
 * Example for using LWIP to create an interlaced gif file.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.writeFile('lena_interlaced.gif', {
        colors: 222,
        interlaced: true
    }, function(err) {
        if (err) return console.log(err);
        console.log('done');
    });
});
