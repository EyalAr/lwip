var path = require('path'),
    fs = require('fs'),
    lwip = require('../'),
    async = require('async'),
    file = path.join(__dirname, 'lena.jpg');

async.waterfall([

    // open image
    function(next) {
        lwip.open(file, next);
    },

    // scale image
    function(image, next) {
        image.resize(25, next);
    },

    // compress to jpeg and get image as buffer
    function(image, next) {
        image.toBuffer('jpg', {
            quality: 60
        }, next);
    },

    function(buffer, next) {
        fs.writeFile('lena_thumb.jpg', buffer, {
            encoding: 'binary',
            flag: 'w'
        }, next);
        // can also, for example, do buffer.toString('base64'), send it over
        // the network and display on a browser... without ever saving the
        // result to disk.
    }

], function(err) {
    if (err) return console.log(err);
    console.log('done')
});
