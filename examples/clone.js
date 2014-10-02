/**
 * Example for using LWIP to clone an image.
 * 1. Do some initial manipulations on the image.
 * 2. Clone 2 separate images.
 * 3. Do some more different manipulations in the clones.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);

    image.batch()
        .scale(0.75)
        .border(5, [50, 100, 75, 75])
        .exec(function(err, image) {
            if (err) return console.log(err);

            image.clone(function(err, clone1) {
                if (err) return console.log("clone1:", err);
                clone1.batch()
                    .mirror('y')
                    .hue(100)
                    .writeFile('lena_clone1.png', function(err) {
                        if (err) return console.log(err);
                        console.log('clone1: done');
                    });
            });

            image.clone(function(err, clone2) {
                if (err) return console.log("clone2:", err);
                clone2.batch()
                    .fade(0.5)
                    .mirror('x')
                    .writeFile('lena_clone2.png', function(err) {
                        if (err) return console.log(err);
                        console.log('clone2: done');
                    });
            });

        });

});
