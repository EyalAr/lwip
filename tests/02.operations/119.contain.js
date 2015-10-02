var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'contain',
    current;

describe('lwip.contain', function() {

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
        image.writeFile(join(tmpDir, current.join('_') + '.gif'), 'gif', {
            colors: 256,
            interlaced: true
        }, done);
    });

    beforeEach(function(){
        current = [ basename ];
    });

    describe('800X100, no color, unspecified interpolation', function() {
        it('image should have the correct size', function(done) {
            current.push('800X100','no_color','unspecified_inter');
            image.contain(800, 100, function(err, im) {
                if (err) return done(err);
                assert(im.width() === 800);
                assert(im.height() === 100);
                done();
            });
        });
    });

    describe('100X800, no color, lanczos interpolation', function() {
        it('image should have the correct size', function(done) {
            current.push('100X800','no_color','lanczos');
            image.contain(100, 800, 'lanczos', function(err, im) {
                if (err) return done(err);
                assert(im.width() === 100);
                assert(im.height() === 800);
                done();
            });
        });
    });

    describe('100X400, gray, unspecified interpolation', function() {
        it('image should have the correct size', function(done) {
            current.push('100X400','gray','unspecified_inter');
            image.contain(100, 400, 'gray', function(err, im) {
                if (err) return done(err);
                assert(im.width() === 100);
                assert(im.height() === 400);
                done();
            });
        });
    });

    describe('400X100, gray, lanczos interpolation', function() {
        it('image should have the correct size', function(done) {
            current.push('400X100','gray','lanczos');
            image.contain(400, 100, 'gray', 'lanczos', function(err, im) {
                if (err) return done(err);
                assert(im.width() === 400);
                assert(im.height() === 100);
                done();
            });
        });
    });

});
