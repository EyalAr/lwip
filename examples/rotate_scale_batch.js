/**
 * Example for using LWIP to scale an image.
 * This example uses the 'async' module for control flow.
 */

var path = require('path'),
    fs = require('fs'),
    lwip = require('../'),
    scaleBy = 0.25,
    rotateBy = 45,
    bgColor = 'white',
    infile = path.join(__dirname, 'lena.jpg'),
    outfile = path.join(__dirname, 'lena_rotated_scaled_down.jpg');

lwip.open(infile, function(err, image) {
    if (err) return console.log(err);
    var batch = image.batch()
        .rotate(rotateBy, bgColor)
        .scale(scaleBy);
    batch.toBuffer('jpg', function(err, buffer) {
        fs.writeFile(outfile, buffer, {
            encoding: 'binary',
            flag: 'w'
        }, function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
