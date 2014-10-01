// methods should throw errors when arguments are invalid

var should = require("should"),
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.paste arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid pasted image (1)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, {}, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (2)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, null, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (3)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (4)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, function() {}, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (5)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, undefined, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (6)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, 0, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (7)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, "foo", function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (8)', function() {
        it('should throw an error', function() {
            image.paste.bind(image, 0, 0, [1, 2, 3], function() {}).should.throwError();
        });
    });

    describe('pasted image exceeds dimensions', function() {

        var Image = require('../../lib/Image');

        var clone;
        before(function(done) {
            image.clone(function(err, img) {
                clone = img;
                done(err);
            });
        });

        it('should throw an error', function() {
            assert(clone instanceof Image);
            image.paste.bind(image, 10, 10, clone, function() {}).should.throwError();
        });
    });
});
