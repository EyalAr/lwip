var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'cover',
    current;

describe('lwip.cover', function() {

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
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpg', {
            quality: 90
        }, done);
    });

    beforeEach(function(){
        current = [ basename ];
    });

    describe('800X300, unspecified interpolation', function() {
        it('image should have the correct size', function(done) {
            current.push('800X300','unspecified_inter');
            image.cover(800, 300, function(err, im) {
                if (err) return done(err);
                assert(im.width() === 800);
                assert(im.height() === 300);
                done();
            });
        });
    });

    describe('300X800, lanczos interpolation', function() {
        it('image should have the correct size', function(done) {
            current.push('300X800','lanczos');
            image.cover(300, 800, 'lanczos', function(err, im) {
                if (err) return done(err);
                assert(im.width() === 300);
                assert(im.height() === 800);
                done();
            });
        });
    });

});
