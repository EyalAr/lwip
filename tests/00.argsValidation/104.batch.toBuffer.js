// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.toBuffer arguments validation', function() {

    var batch;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch().blur(2);
            done(err);
        });
    });

    describe('invalid encoding format', function() {
        it('should throw an error', function() {
            batch.toBuffer.bind(batch, 'jjpeg').should.throwError();
        });
    });

    describe('JPEG params', function() {

        describe('invalid quality', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'jpeg', {
                    quality: 'foo'
                }).should.throwError();
            });
        });

    });

    describe('PNG params', function() {

        describe('invalid compression', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'png', {
                    compression: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid interlaced', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'png', {
                    interlaced: 'foo'
                }, function() {}).should.throwError();
            });
        });

    });

});
