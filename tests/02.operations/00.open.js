var should = require("should"),
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

    describe('jpeg', function() {

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

    describe('png', function() {

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

        describe('invalid image', function() {
            it('should fail', function(done) {
                lwip.open(imgs.png.inv, function(err, img) {
                    should(err).be.Error;
                    done();
                });
            });
        });

    });
});
