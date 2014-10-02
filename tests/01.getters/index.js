var join = require('path').join,
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

var width = 500,
    height = 333;

var image;
before(function(done) {
    lwip.open(imgs.jpg.rgb, function(err, img) {
        image = img;
        done(err);
    });
});

describe('lwip.width', function() {
    it('should return correct width', function() {
        assert(image.width() === width);
    });
});

describe('lwip.height', function() {
    it('should return correct height', function() {
        assert(image.height() === height);
    });
});

describe('lwip.size', function() {
    it('should return correct size', function() {
        assert(image.size().width === width);
        assert(image.size().height === height);
    });
});

describe('lwip.clone', function() {
    it('should return a new image object', function(done) {
        image.clone(function(err, clonedImage) {
            if (err) return done(err);
            clonedImage.resize(100, 100, function(err) {
                if (err) return done(err);
                assert(image.width() === width);
                assert(image.height() === height);
                assert(clonedImage.width() === 100);
                assert(clonedImage.height() === 100);
                done();
            });
        });
    });
});

describe('lwip.extract', function() {
    it('should return a new image object', function(done) {
        image.extract(100, 120, 150, 140, function(err, exImage) {
            if (err) return done(err);
            assert(image.width() === width);
            assert(image.height() === height);
            assert(exImage.width() === 150 - 100 + 1);
            assert(exImage.height() === 140 - 120 + 1);
            done();
        });
    });
});
