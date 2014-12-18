/**
 * Example for using LWIP to cover a canvas with an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.cover(400,800,function(err, image){
        image.writeFile('lena_cover.jpg', function(err){
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
