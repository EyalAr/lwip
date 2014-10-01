/**
 * Example for using LWIP to blend two images.
 */

var path = require('path'),
    async = require('async'),
    lwip = require('../');

async.waterfall([

    function(next) {
        lwip.open('lena.jpg', next);
    },

    function(image, next) {
        image.clone(function(err, clone) {
            next(err, clone, image);
        });
    },

    function(clone, image, next) {
        clone.batch()
            .flip('x')
            .fade(0.5)
            .exec(function(err, clone) {
                next(err, clone, image);
            });
    },

    function(clone, image, next) {
        image.paste(0, 0, clone, next);
    },

    function(image, next) {
        image.writeFile('lena_paste_blend.jpg', next);
    }

], function(err) {
    if (err) return console.log(err);
    console.log('done');
});
