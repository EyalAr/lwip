/**
 * Example for using LWIP to shift image's hue by 50 degrees.
 */

const lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);
    image.writeFile('lena_progressive.jpg', 'jpg', { progressive: true },
        err => {
            if (err) return console.log(err);
            console.log('done');
        });
});
