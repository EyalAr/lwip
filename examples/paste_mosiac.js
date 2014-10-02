/**
 * Example for using LWIP to create a hue gradient mosiac.
 */

var path = require('path'),
    async = require('async'),
    lwip = require('../');

lwip.open('lena.jpg', function(err, image) {
    if (err) return console.log(err);

    image.scale(0.15, function(err, image) {
        if (err) return console.log(err);

        var width = image.width(),
            height = image.height();

        var ROWS = 10,
            COLS = 10,
            HUES = [];

        for (var i = 0; i < ROWS * COLS; i++) {
            HUES.push(i * 360 / ROWS / COLS);
        }

        lwip.create(width * COLS, height * ROWS, function(err, canvas) {
            if (err) return console.log(err);

            var clones = [];
            async.each(HUES, function(h, done) {
                var i = HUES.indexOf(h);
                image.clone(function(err, clone) {
                    if (err) return done(err);
                    var batch = clone.batch();
                    if (i % 2 === 1) batch.mirror('x');
                    if (i % (2 * COLS) >= COLS) batch.mirror('y');
                    batch.hue(h);
                    batch.exec(function(err, clone) {
                        if (err) return done(err);
                        clones[i] = clone;
                        done();
                    });
                });
            }, function(err) {
                if (err) return console.log(err);
                var r = 0,
                    c = 0;
                async.eachSeries(clones, function(clone, next) {
                    canvas.paste(c * width, r * height, clone, function(err) {
                        c++;
                        if (c === COLS) {
                            c = 0;
                            r++;
                        }
                        next(err);
                    });
                }, function(err) {
                    if (err) return console.log(err);
                    canvas.writeFile('lena_paste_mosiac.jpg', function(err) {
                        if (err) return console.log(err);
                        console.log('done');
                    });
                });
            });

        });

    });

});
