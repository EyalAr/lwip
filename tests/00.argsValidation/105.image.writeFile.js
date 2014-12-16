// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.writeFile arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid encoding format', function() {
        it('should throw an error', function() {
            image.writeFile.bind(image, 'res.jpg', 'jjpeg', function() {}).should.throwError();
        });
    });

    describe('JPEG params', function() {

        describe('invalid quality', function() {
            it('should throw an error', function() {
                image.writeFile.bind(image, 'res.jpg', 'jpeg', {
                    quality: 'foo'
                }, function() {}).should.throwError();
            });
        });

    });

    describe('PNG params', function() {

        describe('invalid compression', function() {
            it('should throw an error', function() {
                image.writeFile.bind(image, 'res.jpg', 'png', {
                    compression: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid interlaced', function() {
            it('should throw an error', function() {
                image.writeFile.bind(image, 'res.jpg', 'png', {
                    interlaced: 'foo'
                }, function() {}).should.throwError();
            });
        });

        describe('invalid transparency', function() {
            it('should throw an error', function() {
                image.writeFile.bind(image, 'res.jpg', 'png', {
                    transparency: 'foo'
                }, function() {}).should.throwError();
            });
        });

    });

    describe('GIF params', function() {

        describe("invalid params", function(){

            describe('invalid colors (wrong type)', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        colors: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid colors (<2)', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        colors: 1
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid colors (>256)', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        colors: 257
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid interlaced', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        interlaced: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid transparency', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid threshold (wrong type)', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: true,
                        threshold: 'foo'
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid threshold (<0)', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: true,
                        threshold: -1
                    }, function() {}).should.throwError();
                });
            });

            describe('invalid threshold (>100)', function() {
                it('should throw an error', function() {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: true,
                        threshold: 101
                    }, function() {}).should.throwError();
                });
            });

        });

    });

});
