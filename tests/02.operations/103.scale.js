var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'scale',
    current;

describe('lwip.scale', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.png.trans, function(err, img) {
            image = img;
            done(err);
        });
    });

    afterEach(function(done) {
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', {
            compression: 'high',
            interlaced: true
        }, done);
    });

    describe('width 150%, height 120%', function() {

        var rw = 1.5,
            rh = 1.2,
            width = 0 | rw * 500,
            height = 0 | rh * 333;

        beforeEach(function() {
            current = [basename, 'w' + rw, 'h' + rh];
        });

        describe('unspecified interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('unspecified_inter');
                image.scale(rw, rh, function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('lanczos interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('lanczos');
                image.scale(rw, rh, 'lanczos', function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('cubic interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('cubic');
                image.scale(rw, rh, 'cubic', function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('nearest-neighbor interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('nearest-neighbor');
                image.scale(rw, rh, 'nearest-neighbor', function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('moving-average interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('moving-average');
                image.scale(rw, rh, 'moving-average', function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('linear interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('linear');
                image.scale(rw, rh, 'linear', function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('grid interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('grid');
                image.scale(rw, rh, 'grid', function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

    });

    describe('width and height 25%', function() {

        var r = 0.25,
            width = 0 | r * 500,
            height = 0 | r * 333;

        beforeEach(function() {
            current = [basename, 'r' + r];
        });

        describe('unspecified interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('unspecified_inter');
                image.scale(r, function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

    });

});
