const join = require('path').join,
    should = require("should"),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    outJpeg = 'write_test.jpg',
    outPng = 'write_test.png',
    outGif = 'write_test.gif',
    outpathJpeg = join(tmpDir, outJpeg),
    outpathPng = join(tmpDir, outPng),
    outpathGif = join(tmpDir, outGif);

describe('lwip.writeFile', () => {

    let image;

    before(done => {
        mkdirp(tmpDir, done);
    });
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('jpeg', () => {

        describe('with type unspecified', () => {

            describe('params unspecified', () => {
                it('should succeed', done => {
                    image.writeFile(outpathJpeg, done);
                });
            });

            describe('params specified q=20', () => {
                it('should succeed', done => {
                    image.writeFile(outpathJpeg, {
                        quality: 20
                    }, done);
                });
            });

            describe('params specified q=0', () => {
                it('should succeed', done => {
                    image.writeFile(outpathJpeg, {
                        quality: 0
                    }, done);
                });
            });

        });

        describe('with type specified', () => {

            describe('params unspecified', () => {
                it('should succeed', done => {
                    image.writeFile(outpathJpeg, 'jpeg', done);
                });
            });

            describe('params specified', () => {
                it('should succeed', done => {
                    image.writeFile(outpathJpeg, 'jpeg', {
                        quality: 20
                    }, done);
                });
            });

        });

    });

    describe('png', () => {

        describe('with type unspecified', () => {

            describe('params unspecified', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, done);
                });
            });

            describe('params specified - no compression, not interlaced', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, {
                        compression: 'none',
                        interlaced: false
                    }, done);
                });
            });

            describe('params specified - no compression, interlaced', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, {
                        compression: 'none',
                        interlaced: true
                    }, done);
                });
            });

            describe('params specified - fast compression, not interlaced', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, {
                        compression: 'fast',
                        interlaced: false
                    }, done);
                });
            });

            describe('params specified - fast compression, interlaced', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, {
                        compression: 'fast',
                        interlaced: true
                    }, done);
                });
            });

            describe('params specified - high compression, not interlaced', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, {
                        compression: 'high',
                        interlaced: false
                    }, done);
                });
            });

            describe('params specified - high compression, interlaced', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, {
                        compression: 'high',
                        interlaced: true
                    }, done);
                });
            });

        });

        describe('with type specified', () => {

            describe('params unspecified', () => {
                it('should succeed', done => {
                    image.writeFile(outpathPng, 'png', done);
                });
            });

        });

    });

    describe('gif', () => {

        describe('with type unspecified', () => {

            describe('params unspecified', () => {
                it('should succeed', done => {
                    image.writeFile(outpathGif, done);
                });
            });

            describe('params specified - 256, not interlaced, not transparent, 50', () => {
                it('should succeed', done => {
                    image.writeFile(outpathGif, {
                        colors: 256,
                        interlaced: false,
                        transparent: false,
                        threshold: 100
                    }, done);
                });
            });

            describe('params specified - 99, interlaced, not transparent, 50', () => {
                it('should succeed', done => {
                    image.writeFile(outpathGif, {
                        colors: 99,
                        interlaced: true,
                        transparent: false,
                        threshold: 100
                    }, done);
                });
            });

            describe('params specified - 256, not interlaced, transparent, 30', () => {
                it('should succeed', done => {
                    image.writeFile(outpathGif, {
                        colors: 256,
                        interlaced: false,
                        transparent: true,
                        threshold: 30
                    }, done);
                });
            });

            describe('params specified - 16, interlaced, transparent, 88', () => {
                it('should succeed', done => {
                    image.writeFile(outpathGif, {
                        colors: 16,
                        interlaced: true,
                        transparent: true,
                        threshold: 88
                    }, done);
                });
            });

        });

        describe('with type specified', () => {

            describe('params unspecified', () => {
                it('should succeed', done => {
                    image.writeFile(outpathGif, 'gif', done);
                });
            });

        });

    });
});
