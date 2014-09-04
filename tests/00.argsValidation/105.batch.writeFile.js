// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.writeFile arguments validation', function() {

    var batch;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch().blur(2);
            done(err);
        });
    });

    describe('invalid encoding format', function() {
        it('should throw an error', function() {
            batch.writeFile.bind(batch, 'res.jpg', 'jjpeg').should.throwError();
        });
    });

    describe('JPEG params', function() {

        describe('invalid quality', function() {
            it('should throw an error', function() {
                batch.writeFile.bind(batch, 'res.jpg', 'jpeg', {
                    quality: 'foo'
                }).should.throwError();
            });
        });

    });

    describe('PNG params', function() {

        describe('invalid compression', function() {
            it('should throw an error', function() {
                batch.writeFile.bind(batch, 'res.png', 'png', {
                    compression: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid interlaced', function() {
            it('should throw an error', function() {
                batch.writeFile.bind(batch, 'res.png', 'png', {
                    interlaced: 'foo'
                }, function() {}).should.throwError();
            });
        });

    });

});
