/**
 * Example for using LWIP to manipulate an image and write directly to a file.
 * Steps:
 * 1. Open the file and obtain an 'image' object.
 * 2. Manipulate image
 * 3. Save to disk as a new file.
 */

var path = require('path'),
    fs = require('fs'),
    lwip = require('../'),
    infile = path.join(__dirname, 'lena.jpg'),
    outfile = path.join(__dirname, 'lena_write_file.jpg');

lwip.open(infile, function(err, image) {
    if (err) return console.log(err);
    image.rotate(33, [115, 13, 55], function(err, image) {
        if (err) return console.log(err);
        image.writeFile(outfile, {
            quality: 90
        }, function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
