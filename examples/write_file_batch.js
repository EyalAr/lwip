/**
 * Example for using LWIP batch object to manipulate an image and write
 * directly to a file.
 * Steps:
 * 1. Open the file and obtain a 'batch' object.
 * 2. Queue image manipulations
 * 3. Save to disk as a new file.
 */

var path = require('path'),
    fs = require('fs'),
    lwip = require('../'),
    infile = path.join(__dirname, 'lena.jpg'),
    outfile = path.join(__dirname, 'lena_write_file_batch.jpg');

lwip.open(infile, function(err, image) {
    image.batch().rotate(33, [115, 13, 55]).writeFile(outfile, {
        quality: 90
    }, function(err) {
        if (err) return console.log(err);
        console.log('done');
    });
});
