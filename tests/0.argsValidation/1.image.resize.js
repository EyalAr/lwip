// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.resize arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('with no arguments', function() {
        it('should throw an error', function() {
            image.resize.bind(image).should.throwError();
        });
    });
    describe('with invalid arguments', function() {
        it('should throw an error', function() {
            image.resize.bind(image, 'ten', 'twenty', 'foo', 'bar').should.throwError();
        });
    });
    describe('with no callback', function() {
        it('should throw an error', function() {
            image.resize.bind(image, 100).should.throwError();
        });
    });
    describe('with negative width', function() {
        it('should throw an error', function() {
            image.resize.bind(image, -100, 100, function() {}).should.throwError();
        });
    });
    describe('with negative height', function() {
        it('should throw an error', function() {
            image.resize.bind(image, 100, -100, function() {}).should.throwError();
        });
    });
    describe('without width and height', function() {
        it('should throw an error', function() {
            image.resize.bind(image, function() {}).should.throwError();
        });
    });
    describe('with invalid interpolation', function() {
        it('should throw an error', function() {
            image.resize.bind(image, 100, 100, 'foo', function() {}).should.throwError();
        });
    });
    describe('with invalid height', function() {
        it('should throw an error', function() {
            image.resize.bind(image, 100, 'ten', 'lanczos', function() {}).should.throwError();
        });
    });
});
