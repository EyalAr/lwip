/**
 * Example for using LWIP to blend two images.
 */

const path = require('path'),
    async = require('async'),
    lwip = require('../');

async.waterfall([

    next => {
        lwip.open('lena.jpg', next);
    },

    (image, next) => {
        image.clone((err, clone) => {
            next(err, clone, image);
        });
    },

    (clone, image, next) => {
        clone.batch()
            .flip('x')
            .fade(0.5)
            .exec((err, clone) => {
                next(err, clone, image);
            });
    },

    (clone, image, next) => {
        image.paste(0, 0, clone, next);
    },

    (image, next) => {
        image.writeFile('lena_paste_blend.jpg', next);
    }

], err => {
    if (err) return console.log(err);
    console.log('done');
});
