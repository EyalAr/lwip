/**
 * Example for using LWIP to create a hue gradient mosiac.
 */

const path = require('path'),
    async = require('async'),
    lwip = require('../');

lwip.open('lena.jpg', (err, image) => {
    if (err) return console.log(err);

    image.scale(0.15, (err, image) => {
        if (err) return console.log(err);

        const width = image.width(),
            height = image.height();

        const ROWS = 10,
            COLS = 10,
            HUES = [];

        for (let i = 0; i < ROWS * COLS; i++) {
            HUES.push(i * 360 / ROWS / COLS);
        }

        lwip.create(width * COLS, height * ROWS, (err, canvas) => {
            if (err) return console.log(err);

            const clones = [];
            async.each(HUES, (h, done) => {
                const i = HUES.indexOf(h);
                image.clone((err, clone) => {
                    if (err) return done(err);
                    const batch = clone.batch();
                    if (i % 2 === 1) batch.mirror('x');
                    if (i % (2 * COLS) >= COLS) batch.mirror('y');
                    batch.hue(h);
                    batch.exec((err, clone) => {
                        if (err) return done(err);
                        clones[i] = clone;
                        done();
                    });
                });
            }, err => {
                if (err) return console.log(err);
                let r = 0,
                    c = 0;
                async.eachSeries(clones, (clone, next) => {
                    canvas.paste(c * width, r * height, clone, err => {
                        c++;
                        if (c === COLS) {
                            c = 0;
                            r++;
                        }
                        next(err);
                    });
                }, err => {
                    if (err) return console.log(err);
                    canvas.writeFile('lena_paste_mosiac.jpg', err => {
                        if (err) return console.log(err);
                        console.log('done');
                    });
                });
            });

        });

    });

});
