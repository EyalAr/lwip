/**
 * Example for using LWIP to extract parts of an image.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);

    image.clone(function(err, flipped) {
        if (err) return console.log(err);
        flipped.batch().flip('x').fade(0.5).exec(function(err, flipped) {
            if (err) return console.log(err);
            image.paste(0, 0, flipped, function(err, image) {
                if (err) return console.log(err);
                image.writeFile('lena_paste_blend.jpg', function(err) {
                    if (err) return console.log(err);
                    console.log('done');
                });
            });
        });
    });

});
