var join = require('path').join,
    should = require("should"),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    outJpeg = 'write_test.jpg',
    outPng = 'write_test.png',
    outGif = 'write_test.gif',
    outpathJpeg = join(tmpDir, outJpeg),
    outpathPng = join(tmpDir, outPng),
    outpathGif = join(tmpDir, outGif);

describe('lwip.writeFile', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('jpeg', function() {

        describe('with type unspecified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathJpeg, done);
                });
            });

            describe('params specified q=20', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathJpeg, {
                        quality: 20
                    }, done);
                });
            });

            describe('params specified q=0', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathJpeg, {
                        quality: 0
                    }, done);
                });
            });

        });

        describe('with type specified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathJpeg, 'jpeg', done);
                });
            });

            describe('params specified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathJpeg, 'jpeg', {
                        quality: 20
                    }, done);
                });
            });

        });

    });

    describe('png', function() {

        describe('with type unspecified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, done);
                });
            });

            describe('params specified - no compression, not interlaced', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, {
                        compression: 'none',
                        interlaced: false
                    }, done);
                });
            });

            describe('params specified - no compression, interlaced', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, {
                        compression: 'none',
                        interlaced: true
                    }, done);
                });
            });

            describe('params specified - fast compression, not interlaced', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, {
                        compression: 'fast',
                        interlaced: false
                    }, done);
                });
            });

            describe('params specified - fast compression, interlaced', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, {
                        compression: 'fast',
                        interlaced: true
                    }, done);
                });
            });

            describe('params specified - high compression, not interlaced', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, {
                        compression: 'high',
                        interlaced: false
                    }, done);
                });
            });

            describe('params specified - high compression, interlaced', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, {
                        compression: 'high',
                        interlaced: true
                    }, done);
                });
            });

        });

        describe('with type specified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathPng, 'png', done);
                });
            });

        });

    });

    describe('gif', function() {

        describe('with type unspecified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathGif, done);
                });
            });

            describe('params specified - 256, not interlaced, not transparent, 50', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathGif, {
                        colors: 256,
                        interlaced: false,
                        transparent: false,
                        threshold: 100
                    }, done);
                });
            });

            describe('params specified - 99, interlaced, not transparent, 50', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathGif, {
                        colors: 99,
                        interlaced: true,
                        transparent: false,
                        threshold: 100
                    }, done);
                });
            });

            describe('params specified - 256, not interlaced, transparent, 30', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathGif, {
                        colors: 256,
                        interlaced: false,
                        transparent: true,
                        threshold: 30
                    }, done);
                });
            });

            describe('params specified - 16, interlaced, transparent, 88', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathGif, {
                        colors: 16,
                        interlaced: true,
                        transparent: true,
                        threshold: 88
                    }, done);
                });
            });

        });

        describe('with type specified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpathGif, 'gif', done);
                });
            });

        });

    });
});
