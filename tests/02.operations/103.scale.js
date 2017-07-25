const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'scale';
let current;

describe('lwip.scale', () => {

    let image;

    before(done => {
        mkdirp(tmpDir, done);
    });

    beforeEach(done => {
        lwip.open(imgs.png.trans, (err, img) => {
            image = img;
            done(err);
        });
    });

    afterEach(done => {
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', {
            compression: 'high',
            interlaced: true
        }, done);
    });

    describe('width 150%, height 120%', () => {

        const rw = 1.5,
            rh = 1.2,
            width = 0 | rw * 500,
            height = 0 | rh * 333;

        beforeEach(() => {
            current = [basename, 'w' + rw, 'h' + rh];
        });

        describe('unspecified interpolation', () => {
            it('image should have the correct size', done => {
                current.push('unspecified_inter');
                image.scale(rw, rh, (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('lanczos interpolation', () => {
            it('image should have the correct size', done => {
                current.push('lanczos');
                image.scale(rw, rh, 'lanczos', (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('cubic interpolation', () => {
            it('image should have the correct size', done => {
                current.push('cubic');
                image.scale(rw, rh, 'cubic', (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('nearest-neighbor interpolation', () => {
            it('image should have the correct size', done => {
                current.push('nearest-neighbor');
                image.scale(rw, rh, 'nearest-neighbor', (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('moving-average interpolation', () => {
            it('image should have the correct size', done => {
                current.push('moving-average');
                image.scale(rw, rh, 'moving-average', (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('linear interpolation', () => {
            it('image should have the correct size', done => {
                current.push('linear');
                image.scale(rw, rh, 'linear', (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('grid interpolation', () => {
            it('image should have the correct size', done => {
                current.push('grid');
                image.scale(rw, rh, 'grid', (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

    });

    describe('width and height 25%', () => {

        const r = 0.25,
            width = 0 | r * 500,
            height = 0 | r * 333;

        beforeEach(() => {
            current = [basename, 'r' + r];
        });

        describe('unspecified interpolation', () => {
            it('image should have the correct size', done => {
                current.push('unspecified_inter');
                image.scale(r, (err, im) => {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

    });

});
