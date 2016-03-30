/**
 * Example for using LWIP to shift image's hue by 50 degrees.
 */

var path = require('path'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);
    image.writeFile('lena_progressive.jpg', 'jpg', { progressive: true },
        function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
});
