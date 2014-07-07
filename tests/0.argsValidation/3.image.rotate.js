// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.rotate arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('with no arguments', function() {
        it('should throw an error', function() {
            image.rotate.bind(image).should.throwError();
        });
    });
    describe('with invalid arguments', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, '10degs', ['a', 'b', 'c'], 'foo').should.throwError();
        });
    });
    describe('with no callback', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 45, 'white').should.throwError();
        });
    });
    describe('with no color and no callback', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 45).should.throwError();
        });
    });
    describe('with invalid callback argument', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, 'white', 'foo').should.throwError();
        });
    });
    describe('with invalid color object', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, {
                foo: 'bar',
                bar: 'foo'
            }, function() {}).should.throwError();
        });
    });
    describe('with invalid color array', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, ['a', 'b'], function() {}).should.throwError();
        });
    });
    describe('with invalid color array', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, ['a', 'b', 'c'], function() {}).should.throwError();
        });
    });
    describe('with invalid color string', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, 'foo', function() {}).should.throwError();
        });
    });
    describe('with invalid color argument type', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, 99, function() {}).should.throwError();
        });
    });
});
