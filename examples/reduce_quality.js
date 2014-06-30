/**
 * Example for using LWIP to reduce the quality of a JPEG file.
 * Steps:
 * 1. Open the file and obtain an 'image' object.
 * 2. Generate a buffer of the image compressed as JPEG at quality 10%.
 * 3. Save buffer to disk as a new file.
*/

var path = require('path'),
    fs = require('fs'),
    lwip = require('../'),
    jpegQuality = 10,
    infile = path.join(__dirname, 'lena.jpg'),
    outfile = path.join(__dirname, 'lena_low_quality.jpg');

lwip.open(infile, function(err, image) {
    if (err) return console.log(err);
    image.toBuffer('jpg', {
        quality: jpegQuality
    }, function(err, buffer) {
        if (err) return console.log(err);
        fs.writeFile(outfile, buffer, {
            encoding: 'binary',
            flag: 'w'
        }, function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
