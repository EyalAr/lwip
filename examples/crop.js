/**
 * Example for using LWIP to crop an image.
 */

var path = require('path'),
    lwip = require('../'),
    infile = path.join(__dirname, 'lena.jpg'),
    outfile = path.join(__dirname, 'lena_crop.jpg');

lwip.open(infile, function(err, image) {
    if (err) return console.log(err);
    image.batch().crop(500).writeFile(outfile, function(err) {
        if (err) return console.log(err);
        console.log('done');
    });
});
