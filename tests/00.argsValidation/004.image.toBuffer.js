// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.toBuffer arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid encoding format', function() {
        it('should throw an error', function() {
            image.toBuffer.bind(image, 'jjpeg', function() {}).should.throwError();
        });
    });

    describe('JPEG params', function() {

        describe('invalid quality', function() {
            it('should throw an error', function() {
                image.toBuffer.bind(image, 'jpeg', {
                    quality: 'foo'
                }, function() {}).should.throwError();
            });
        });

    });

    describe('PNG params', function() {

        describe('invalid compression', function() {
            it('should throw an error', function() {
                image.toBuffer.bind(image, 'png', {
                    compression: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid interlaced', function() {
            it('should throw an error', function() {
                image.toBuffer.bind(image, 'png', {
                    interlaced: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid transparency', function() {
            it('should throw an error', function() {
                image.toBuffer.bind(image, 'png', {
                    transparency: 'foo'
                }, function() {}).should.throwError();
            });
        });

    });

});
