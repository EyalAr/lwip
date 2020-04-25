const join = require('path').join,
    fs = require('fs'),
    async = require('async'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    utils = require('../utils'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results'),
    outpathJpeg = join(tmpDir, 'stress.jpg'),
    outpathPng = join(tmpDir, 'stress.png'),
    outpathGif = join(tmpDir, 'stress.gif');

describe('stress tests', function () {

    this.timeout(120000); // 120 seconds per test

    before(() => {
        return mkdirp(tmpDir);
    });

    describe('open image 300 times (in parallel) and save to disk as jpeg', () => {
        it('should succeed', done => {
            async.times(300, (i, done) => {
                lwip.open(imgs.png.rgb, 'png', (err, image) => {
                    if (err) return done(err);
                    image.writeFile(outpathJpeg, 'jpeg', {
                        quality: 50
                    }, done);
                });
            }, done);
        });
    });

    describe('open image 100 times (in parallel) and save to disk as png (high compression, interlaced)', () => {
        it('should succeed', done => {
            async.times(100, (i, done) => {
                lwip.open(imgs.jpg.rgb, 'jpeg', (err, image) => {
                    if (err) return done(err);
                    image.writeFile(outpathPng, 'png', {
                        compression: 'high',
                        interlaced: true
                    }, done);
                });
            }, done);
        });
    });

    describe('open image 300 times (in parallel) and save to disk as png (fast compression, not interlaced)', () => {
        it('should succeed', done => {
            async.times(300, (i, done) => {
                lwip.open(imgs.gif.rgb, 'gif', (err, image) => {
                    if (err) return done(err);
                    image.writeFile(outpathPng, 'png', {
                        compression: 'fast',
                        interlaced: false
                    }, done);
                });
            }, done);
        });
    });

    describe('open image 300 times (in parallel) and save to disk as gif (128 colors, not interlaced, transparent)', () => {
        it('should succeed', done => {
            async.times(300, (i, done) => {
                lwip.open(imgs.png.trans, 'png', (err, image) => {
                    if (err) return done(err);
                    image.writeFile(outpathGif, 'gif', {
                        colors: 128,
                        transparency: true,
                        threshold: 60
                    }, done);
                });
            }, done);
        });
    });

    describe('7 random manipulations for 50 images (in parallel)', () => {
        it('should succeed', done => {
            async.times(50, (i, done) => {
                lwip.open(imgs.jpg.rgb, 'jpeg', (err, image) => {
                    if (err) return done(err);
                    const batch = image.batch();
                    const ops = utils.generateRandomBatch(batch, 7);
                    batch.writeFile(join(tmpDir, 'stress-rnd-' + i + '.jpg'), 'jpeg', {
                        quality: 50
                    }, err => {
                        if (err) return done(err);
                        const data = ops.join('\n');
                        fs.writeFile(join(tmpDir, 'stress-rnd-' + i + '.txt'), data, done);
                    });
                });
            }, done);
        });
    });

    describe('rotate an image 30 times (up to 90degs) (1)', () => {
        it('should succeed', done => {
            const a = 3;
            lwip.open(imgs.jpg.rgb, 'jpeg', (err, image) => {
                if (err) return done(err);
                async.timesSeries(90 / a, (i, done) => {
                    image.rotate(a, utils.getRandomColor(), done);
                }, err => {
                    if (err) return done(err);
                    image.writeFile(outpathJpeg, 'jpeg', {
                        quality: 90
                    }, done);
                });
            });
        });
    });

    describe('rotate an image 30 times (up to 90degs) (2)', () => {
        it('should succeed', done => {
            const a = 3;
            lwip.open(imgs.png.trans, 'png', (err, image) => {
                if (err) return done(err);
                async.timesSeries(90 / a, (i, done) => {
                    image.rotate(a, utils.getRandomColor(), done);
                }, err => {
                    if (err) return done(err);
                    image.writeFile(outpathPng, 'png', {
                        compression: 'fast',
                        interlaced: false
                    }, done);
                });
            });
        });
    });

    describe('25 random manipulations on one image (1)', () => {
        it('should succeed', done => {
            lwip.open(imgs.png.rgb, 'png', (err, image) => {
                if (err) return done(err);
                const batch = image.batch();
                const ops = utils.generateRandomBatch(batch, 25);
                batch.writeFile(join(tmpDir, 'stress-25rnd.jpg'), 'jpeg', {
                    quality: 85
                }, err => {
                    if (err) return done(err);
                    const data = ops.join('\n');
                    fs.writeFile(join(tmpDir, 'stress-25rnd.jpg.txt'), data, done);
                });
            });
        });
    });

    describe('25 random manipulations on one image (2)', () => {
        it('should succeed', done => {
            lwip.open(imgs.png.trans, 'png', (err, image) => {
                if (err) return done(err);
                const batch = image.batch();
                const ops = utils.generateRandomBatch(batch, 25);
                batch.writeFile(join(tmpDir, 'stress-25rnd.png'), 'png', {
                    compression: 'fast',
                    interlaced: false
                }, err => {
                    if (err) return done(err);
                    const data = ops.join('\n');
                    fs.writeFile(join(tmpDir, 'stress-25rnd.png.txt'), data, done);
                });
            });
        });
    });

});
