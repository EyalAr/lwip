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

describe('lwip.getPixel', function() {
    it('should return correct color at 0,0', function() {
        var color = image.getPixel(0, 0);
        assert(color.r === 48);
        assert(color.g === 86);
        assert(color.b === 151);
        assert(color.a === 100);
    });

    it('should return correct color at 418, 242', function() {
        var color = image.getPixel(418, 242);
        assert(color.r === 208);
        assert(color.g === 228);
        assert(color.b === 237);
        assert(color.a === 100);
    });

    it('should return correct color at 499, 332', function() {
        var color = image.getPixel(499, 332);
        assert(color.r === 31);
        assert(color.g === 27);
        assert(color.b === 0);
        assert(color.a === 100);
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

describe('lwip.getMetadata', function() {
    it('should return correct metadata', function(done) {
        lwip.open(imgs.png.hasMetadata, function(err, img) {
            assert(img.getMetadata() === "Lorem ipsum dolor sit amet");
            done();
        });
    });

    it('should return null if no metadata found', function(done) {
        lwip.open(imgs.png.noMetadata, function(err, img) {
            assert(img.getMetadata() === null);
            done();
        });
    });
});
