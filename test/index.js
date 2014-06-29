var path = require('path'),
    fs = require('fs'),
    lwip = require('../'),
    file = path.join(__dirname, 'lena.jpg');
lwip.open(file, function(err, image) {
    if (err) return console.log(err);
    console.log(image.width(), image.height());
    image.toBuffer('jpg', function(err, buffer) {
        if (err) return console.log(err);
        fs.writeFile('lena2.jpg', buffer, {
            encoding: 'binary',
            flag: 'w'
        }, function(err) {
            if (err) return console.log(err);
            console.log('done');
        });
    });
});
