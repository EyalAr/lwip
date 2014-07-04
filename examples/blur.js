/**
 * Example for using LWIP to blur an image.
 */

var path = require('path'),
    lwip = require('../'),
    infile = path.join(__dirname, 'lena.jpg'),
    outfile = path.join(__dirname, 'lena_blur.jpg');

lwip.open(infile, function(err, image) {
    if (err) return console.log(err);
    image.batch().blur(10).writeFile(outfile, function(err) {
        if (err) return console.log(err);
        console.log('done');
    });
});
