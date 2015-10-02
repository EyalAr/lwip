var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'crop',
    current;

describe('lwip.crop', function() {

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
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('rectangle from center', function() {

        beforeEach(function() {
            current = [basename, 'center'];
        });

        describe('100X100', function() {
            var width = 100,
                height = 100;
            it('image should have the correct size', function(done) {
                current.push(width + 'X' + height);
                image.crop(width, height, function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

        describe('200X100', function() {
            var width = 200,
                height = 100;
            it('image should have the correct size', function(done) {
                current.push(width + 'X' + height);
                image.crop(width, height, function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

    });

    describe('rectangle coordinates', function() {

        beforeEach(function() {
            current = [basename, 'coordinates'];
        });

        describe('10,10,100,100', function() {
            var left = 10,
                top = 10,
                right = 100,
                bottom = 100,
                width = right - left + 1,
                height = bottom - top + 1;
            it('image should have the correct size', function(done) {
                current.push([left, top, right, bottom].join(','));
                image.crop(left, top, right, bottom, function(err, im) {
                    if (err) return done(err);
                    assert(im.width() === width);
                    assert(im.height() === height);
                    done();
                });
            });
        });

    });

});
