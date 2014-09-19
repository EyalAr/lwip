var should = require("should"),
    fs = require('fs'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.open', function() {
    describe('non-existing image', function() {
        it('should fail', function(done) {
            lwip.open(imgs.inv, 'jpg', function(err, img) {
                should(err).be.Error;
                done();
            });
        });
    });

    describe('jpeg file', function() {

        describe('rgb image (with jpg extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.jpg.rgb, function(err, img) {
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

        describe('rgb image (no extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.jpg.noex, 'jpg', function(err, img) {
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.jpg.gs, function(err, img) {
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

        describe('invalid image', function() {
            it('should fail', function(done) {
                lwip.open(imgs.jpg.inv, function(err, img) {
                    should(err).be.Error;
                    done();
                });
            });
        });

    });

    describe('png file', function() {

        describe('rgb image (with png extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.rgb, function(err, img) {
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

        describe('rgb image (no extension)', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.noex, 'png', function(err, img) {
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

        describe('grayscale image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.gs, function(err, img) {
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

        describe('transparent image', function() {
            it('should succeed', function(done) {
                lwip.open(imgs.png.trans, function(err, img) {
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

        describe('invalid image', function() {
            it('should fail', function(done) {
                lwip.open(imgs.png.inv, function(err, img) {
                    should(err).be.Error;
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
                    should(err).not.be.Error;
                    img.should.be.OK;
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
                    should(err).not.be.Error;
                    img.should.be.OK;
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
                    should(err).not.be.Error;
                    img.should.be.OK;
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
                    should(err).not.be.Error;
                    img.should.be.OK;
                    done();
                });
            });
        });

    });
});
