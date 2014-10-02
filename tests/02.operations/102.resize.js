var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'resize',
    current;

describe('lwip.resize', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    afterEach(function(done) {
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('1000X1330', function() {

        var width = 1000,
            height = 1330;

        beforeEach(function() {
            current = [basename, width + 'X' + height];
        });

        describe('unspecified interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('unspecified_inter');
                image.resize(width, height, function(err, im) {
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
                image.resize(width, height, 'lanczos', function(err, im) {
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
                image.resize(width, height, 'cubic', function(err, im) {
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
                image.resize(width, height, 'nearest-neighbor', function(err, im) {
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
                image.resize(width, height, 'moving-average', function(err, im) {
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
                image.resize(width, height, 'linear', function(err, im) {
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
                image.resize(width, height, 'grid', function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

    });

    describe('100X100', function() {

        var size = 100;

        beforeEach(function() {
            current = [basename, size + 'X' + size];
        });

        describe('unspecified interpolation', function() {
            it('image should have the correct size', function(done) {
                current.push('unspecified_inter');
                image.resize(size, function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === size);
                    assert(im.height() === size);
                    done();
                });
            });
        });

    });

});
