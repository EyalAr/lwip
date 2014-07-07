// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.crop arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('with no arguments', function() {
        it('should throw an error', function() {
            image.crop.bind(image).should.throwError();
        });
    });
    describe('with invalid arguments', function() {
        it('should throw an error', function() {
            image.crop.bind(image, 'foo', [1, 2, 3], 'bar', {}, 'hello').should.throwError();
        });
    });
    describe('with no callback', function() {
        it('should throw an error', function() {
            image.crop.bind(image, 10, 10, 200, 200).should.throwError();
        });
    });
    describe('with invalid callback', function() {
        it('should throw an error', function() {
            image.crop.bind(image, 10, 10, 200, 200, {}).should.throwError();
        });
    });

    describe('with rectangle coordinates', function() {


        describe('with no color and no callback', function() {
            it('should throw an error', function() {
                image.crop.bind(image, 45).should.throwError();
            });
        });
        describe('with invalid callback argument', function() {
            it('should throw an error', function() {
                image.crop.bind(image, 5, 'white', 'foo').should.throwError();
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

    });

});
