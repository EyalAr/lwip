var should = require("should"),
    fs = require('fs'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.open', function() {
    describe('non-existing image', function() {
        it('should fail', function(done) {
            lwip.open(imgs.inv, 'jpg', function(err, img) {
                should(err).be.Error();
                done();
            });
        });
    });

    describe('jpeg file', function() {

        describe('rgb image (with jpg extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.jpg.rgb, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('rgb image (no extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.jpg.noex, 'jpg', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.jpg.gs, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('invalid image', function() {
            it('should fail', function(done) {
                lwip.open(imgs.jpg.inv, function(err, img) {
                    should(err).be.Error();
                    done();
                });
            });
        });

    });

    describe('png file', function() {

        describe('rgb image (with png extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.rgb, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('rgb image (no extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.noex, 'png', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.gs, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('transparent image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.trans, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('invalid image', function() {
            it('should fail', function(done) {
                lwip.open(imgs.png.inv, function(err, img) {
                    should(err).be.Error();
                    done();
                });
            });
        });

    });

    describe('gif file', function() {

        describe('rgb image (with gif extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.gif.rgb, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('rgb image (no extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.gif.noex, 'gif', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.gif.gs, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('transparent image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.gif.trans, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('invalid image', function() {
            it('should fail', function(done) {
                lwip.open(imgs.gif.inv, function(err, img) {
                    should(err).be.Error();
                    done();
                });
            });
        });

    });

    describe('jpeg buffer', function() {

        describe('rgb image', function() {
            var buffer;
            before(function(done) {
                fs.readFile(imgs.jpg.rgb, function(err, imbuf) {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', function(done) {
                lwip.open(buffer, 'jpg', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            var buffer;
            before(function(done) {
                fs.readFile(imgs.jpg.gs, function(err, imbuf) {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', function(done) {
                lwip.open(buffer, 'jpg', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

    });

    describe('png buffer', function() {

        describe('rgb image', function() {
            var buffer;
            before(function(done) {
                fs.readFile(imgs.png.rgb, function(err, imbuf) {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', function(done) {
                lwip.open(buffer, 'png', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            var buffer;
            before(function(done) {
                fs.readFile(imgs.png.gs, function(err, imbuf) {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', function(done) {
                lwip.open(buffer, 'png', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

    });

    describe('gif buffer', function() {

        describe('rgb image', function() {
            var buffer;
            before(function(done) {
                fs.readFile(imgs.gif.rgb, function(err, imbuf) {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', function(done) {
                lwip.open(buffer, 'gif', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            var buffer;
            before(function(done) {
                fs.readFile(imgs.gif.gs, function(err, imbuf) {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', function(done) {
                lwip.open(buffer, 'gif', function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

    });

    describe('raw pixel buffer', function() {

        describe('grayscale image', function(){
            var buffer;
            before(function(done) {
                buffer = new Buffer(100 * 100);
                buffer.fill(0);
                done();
            });

            it('should construct an all-black pixels image', function(done) {
                lwip.open(buffer, { width: 100, height: 100 }, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (var x = 0 ; x < img.width() ; x++){
                        for (var y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 0, g: 0, b: 0, a: 100});
                        }
                    }
                    done();
                });
            });
        });

        describe('grayscale image with alpha', function(){
            var buffer;
            before(function(done) {
                buffer = new Buffer(100 * 100 * 2);
                buffer.fill(0, 0, 10000);
                buffer.fill(50, 10000);
                done();
            });

            it('should construct an all-black transparent pixels image', function(done) {
                lwip.open(buffer, { width: 100, height: 100 }, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (var x = 0 ; x < img.width() ; x++){
                        for (var y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 0, g: 0, b: 0, a: 50});
                        }
                    }
                    done();
                });
            });
        });

        describe('rgb image', function(){
            var buffer;
            before(function(done) {
                buffer = new Buffer(100 * 100 * 3);
                buffer.fill(10, 0, 10000);
                buffer.fill(20, 10000, 2 * 10000);
                buffer.fill(30, 2 * 10000);
                done();
            });

            it('should construct an all-black pixels image', function(done) {
                lwip.open(buffer, { width: 100, height: 100 }, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (var x = 0 ; x < img.width() ; x++){
                        for (var y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 10, g: 20, b: 30, a: 100});
                        }
                    }
                    done();
                });
            });
        });

        describe('rgb image with alpha', function(){
            var buffer;
            before(function(done) {
                buffer = new Buffer(100 * 100 * 4);
                buffer.fill(10, 0, 10000);
                buffer.fill(20, 10000, 2 * 10000);
                buffer.fill(30, 2 * 10000, 3 * 10000);
                buffer.fill(50, 3 * 10000);
                done();
            });

            it('should construct an all-black transparent pixels image', function(done) {
                lwip.open(buffer, { width: 100, height: 100 }, function(err, img) {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (var x = 0 ; x < img.width() ; x++){
                        for (var y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 10, g: 20, b: 30, a: 50});
                        }
                    }
                    done();
                });
            });
        });

    });
});
