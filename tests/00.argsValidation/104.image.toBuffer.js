// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.toBuffer arguments validation', function() {

    var image;
    beforeEach(function(done) {
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

        describe("valid params", function(){

            describe('defaults', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', done).should.not.throwError();
                });
            });

            describe('none, false, true', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: false,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('fast, false, true', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: false,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('high, false, true', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: false,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('none, true, true', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: true,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('fast, true, true', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: true,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('high, true, true', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: true,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('none, false, false', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: false,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('fast, false, false', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: false,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('high, false, false', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: false,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('none, true, false', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: true,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('fast, true, false', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: true,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('high, true, false', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: true,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('none, false, auto', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: false,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('fast, false, auto', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: false,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('high, false, auto', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: false,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('none, true, auto', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: true,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('fast, true, auto', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: true,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('high, true, auto', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: true,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

        });

        describe("invalid params", function(){

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

    describe('GIF params', function() {

        describe("valid params", function(){

            describe('defaults', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'gif', done).should.not.throwError();
                });
            });

            describe('120, false, false, 50', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 120,
                        interlaced: false,
                        transparency: false,
                        threshold: 50
                    }, done).should.not.throwError();
                });
            });

            describe('256, true, false, 50', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 256,
                        interlaced: true,
                        transparency: false,
                        threshold: 50
                    }, done).should.not.throwError();
                });
            });

            describe('2, false, true, 0', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 2,
                        interlaced: false,
                        transparency: true,
                        threshold: 0
                    }, done).should.not.throwError();
                });
            });

            describe('120, true, true, 100', function() {
                it('should succeed', function(done) {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 120,
                        interlaced: true,
                        transparency: true,
                        threshold: 100
                    }, done).should.not.throwError();
                });
            });

        });

        describe("invalid params", function(){

            describe('invalid colors (wrong type)', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid colors (<2)', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 1
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid colors (>256)', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 257
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid interlaced', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        interlaced: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid transparency', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid threshold (wrong type)', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: true,
                        threshold: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid threshold (<0)', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: true,
                        threshold: -1
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid threshold (>100)', function() {
                it('should throw an error', function() {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: true,
                        threshold: 101
                    }, function() {}).should.throwError();
                });
            });

        });

    });

});
