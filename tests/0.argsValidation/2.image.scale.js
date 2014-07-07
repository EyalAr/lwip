// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.scale arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('with no arguments', function() {
        it('should throw an error', function() {
            image.scale.bind(image).should.throwError();
        });
    });
    describe('with invalid arguments', function() {
        it('should throw an error', function() {
            image.scale.bind(image, 'two', 'half', 'foo', 'bar').should.throwError();
        });
    });
    describe('with no callback', function() {
        it('should throw an error', function() {
            image.scale.bind(image, 0.5).should.throwError();
        });
    });
    describe('with negative width ratio', function() {
        it('should throw an error', function() {
            image.scale.bind(image, -0.5, 0.5, function() {}).should.throwError();
        });
    });
    describe('with negative height ratio', function() {
        it('should throw an error', function() {
            image.scale.bind(image, 0.5, -0.5, function() {}).should.throwError();
        });
    });
    describe('without width and height ratios', function() {
        it('should throw an error', function() {
            image.scale.bind(image, function() {}).should.throwError();
        });
    });
    describe('with invalid interpolation', function() {
        it('should throw an error', function() {
            image.scale.bind(image, 0.5, 0.5, 'foo', function() {}).should.throwError();
        });
    });
    describe('with invalid height ratio', function() {
        it('should throw an error', function() {
            image.scale.bind(image, 0.5, 'half', 'lanczos', function() {}).should.throwError();
        });
    });
});
