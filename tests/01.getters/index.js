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
