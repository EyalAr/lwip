var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'pad',
    current = [basename];

describe('lwip.pad', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.png.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    afterEach(function(done) {
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('left-1, top-3, right-5, bottom-8, color-green', function() {
        var width = 500 + 1 + 5,
            height = 333 + 3 + 8;
        it('image should have the correct size', function(done) {
            current.push('l1_t3_r5_b8_green');
            image.pad(1, 3, 5, 8, 'green', function(err, im) {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

});
