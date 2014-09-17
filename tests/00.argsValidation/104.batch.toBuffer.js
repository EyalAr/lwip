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

        describe('invalid quality: foo', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'jpeg', {
                    quality: 'foo'
                }).should.throwError();
            });
        });

        describe('invalid quality: -1', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'jpeg', {
                    quality: -1
                }).should.throwError();
            });
        });

        describe('invalid quality: 101', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'jpeg', {
                    quality: 101
                }).should.throwError();
            });
        });

    });

    describe('PNG params', function() {

        describe('invalid compression - string', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'png', {
                    compression: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid compression - number', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'png', {
                    compression: 98
                }, function() {}).should.throwError();
            });
        });

        describe('invalid interlaced - string', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'png', {
                    interlaced: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid interlaced - number', function() {
            it('should throw an error', function() {
                batch.toBuffer.bind(batch, 'png', {
                    interlaced: 55
                }, function() {}).should.throwError();
            });
        });

    });

});
