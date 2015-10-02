var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'setPixel',
    current;

describe('lwip.setPixel', function() {

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

    beforeEach(function() {
        current = [basename];
    });

    describe('red pixel at 0,0', function() {

        it('pixel should have the correct color', function(done) {
            current.push('0,0-red');
            image.setPixel(0, 0, 'red', function(err, im) {
                if (err) return done(err);
                var color = im.getPixel(0, 0);
                assert(color.r === 255);
                assert(color.g === 0);
                assert(color.b === 0);
                assert(color.a === 100);
                done();
            });
        });

    });

    describe('red pixel at 100,100', function() {

        it('pixel should have the correct color', function(done) {
            current.push('100,100-red');
            image.setPixel(100, 100, 'red', function(err, im) {
                if (err) return done(err);
                var color = im.getPixel(100, 100);
                assert(color.r === 255);
                assert(color.g === 0);
                assert(color.b === 0);
                assert(color.a === 100);
                done();
            });
        });

    });

});
