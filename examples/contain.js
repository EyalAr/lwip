/**
 * Example for using LWIP to contain an image in a canvas.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.contain(400,700,'green',function(err, image){
        image.writeFile('lena_contain.jpg', function(err){
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
