const should = require("should"),
    fs = require('fs'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.open', () => {
    describe('non-existing image', () => {
        it('should fail', done => {
            lwip.open(imgs.inv, 'jpg', (err, img) => {
                should(err).be.Error();
                done();
            });
        });
    });

    describe('jpeg file', () => {

        describe('rgb image (with jpg extension)', () => {
            it('should succeed', done => {
                lwip.open(imgs.jpg.rgb, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('rgb image (no extension)', () => {
            it('should succeed', done => {
                lwip.open(imgs.jpg.noex, 'jpg', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', () => {
            it('should succeed', done => {
                lwip.open(imgs.jpg.gs, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('invalid image', () => {
            it('should fail', done => {
                lwip.open(imgs.jpg.inv, (err, img) => {
                    should(err).be.Error();
                    done();
                });
            });
        });

    });

    describe('png file', () => {

        describe('rgb image (with png extension)', () => {
            it('should succeed', done => {
                lwip.open(imgs.png.rgb, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('rgb image (no extension)', () => {
            it('should succeed', done => {
                lwip.open(imgs.png.noex, 'png', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', () => {
            it('should succeed', done => {
                lwip.open(imgs.png.gs, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('transparent image', () => {
            it('should succeed', done => {
                lwip.open(imgs.png.trans, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('invalid image', () => {
            it('should fail', done => {
                lwip.open(imgs.png.inv, (err, img) => {
                    should(err).be.Error();
                    done();
                });
            });
        });

    });

    describe('gif file', () => {

        describe('rgb image (with gif extension)', () => {
            it('should succeed', done => {
                lwip.open(imgs.gif.rgb, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('rgb image (no extension)', () => {
            it('should succeed', done => {
                lwip.open(imgs.gif.noex, 'gif', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', () => {
            it('should succeed', done => {
                lwip.open(imgs.gif.gs, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('transparent image', () => {
            it('should succeed', done => {
                lwip.open(imgs.gif.trans, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('invalid image', () => {
            it('should fail', done => {
                lwip.open(imgs.gif.inv, (err, img) => {
                    should(err).be.Error();
                    done();
                });
            });
        });

    });

    describe('jpeg buffer', () => {

        describe('rgb image', () => {
            let buffer;
            before(done => {
                fs.readFile(imgs.jpg.rgb, (err, imbuf) => {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', done => {
                lwip.open(buffer, 'jpg', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', () => {
            let buffer;
            before(done => {
                fs.readFile(imgs.jpg.gs, (err, imbuf) => {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', done => {
                lwip.open(buffer, 'jpg', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

    });

    describe('png buffer', () => {

        describe('rgb image', () => {
            let buffer;
            before(done => {
                fs.readFile(imgs.png.rgb, (err, imbuf) => {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', done => {
                lwip.open(buffer, 'png', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', () => {
            let buffer;
            before(done => {
                fs.readFile(imgs.png.gs, (err, imbuf) => {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', done => {
                lwip.open(buffer, 'png', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

    });

    describe('gif buffer', () => {

        describe('rgb image', () => {
            let buffer;
            before(done => {
                fs.readFile(imgs.gif.rgb, (err, imbuf) => {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', done => {
                lwip.open(buffer, 'gif', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

        describe('grayscale image', () => {
            let buffer;
            before(done => {
                fs.readFile(imgs.gif.gs, (err, imbuf) => {
                    buffer = imbuf;
                    done(err);
                });
            });

            it('should succeed', done => {
                lwip.open(buffer, 'gif', (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    done();
                });
            });
        });

    });

    describe('raw pixel buffer', () => {

        describe('grayscale image', () => {
            let buffer;
            before(done => {
                buffer = new Buffer(100 * 100);
                buffer.fill(0);
                done();
            });

            it('should construct an all-black pixels image', done => {
                lwip.open(buffer, { width: 100, height: 100 }, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (let x = 0 ; x < img.width() ; x++){
                        for (let y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 0, g: 0, b: 0, a: 100});
                        }
                    }
                    done();
                });
            });
        });

        describe('grayscale image with alpha', () => {
            let buffer;
            before(done => {
                buffer = new Buffer(100 * 100 * 2);
                buffer.fill(0, 0, 10000);
                buffer.fill(50, 10000);
                done();
            });

            it('should construct an all-black transparent pixels image', done => {
                lwip.open(buffer, { width: 100, height: 100 }, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (let x = 0 ; x < img.width() ; x++){
                        for (let y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 0, g: 0, b: 0, a: 50});
                        }
                    }
                    done();
                });
            });
        });

        describe('rgb image', () => {
            let buffer;
            before(done => {
                buffer = new Buffer(100 * 100 * 3);
                buffer.fill(10, 0, 10000);
                buffer.fill(20, 10000, 2 * 10000);
                buffer.fill(30, 2 * 10000);
                done();
            });

            it('should construct an all-black pixels image', done => {
                lwip.open(buffer, { width: 100, height: 100 }, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (let x = 0 ; x < img.width() ; x++){
                        for (let y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 10, g: 20, b: 30, a: 100});
                        }
                    }
                    done();
                });
            });
        });

        describe('rgb image with alpha', () => {
            let buffer;
            before(done => {
                buffer = new Buffer(100 * 100 * 4);
                buffer.fill(10, 0, 10000);
                buffer.fill(20, 10000, 2 * 10000);
                buffer.fill(30, 2 * 10000, 3 * 10000);
                buffer.fill(50, 3 * 10000);
                done();
            });

            it('should construct an all-black transparent pixels image', done => {
                lwip.open(buffer, { width: 100, height: 100 }, (err, img) => {
                    should(err).not.be.Error();
                    img.should.be.ok();
                    for (let x = 0 ; x < img.width() ; x++){
                        for (let y = 0 ; y < img.height() ; y++){
                            img.getPixel(x, y).should.deepEqual({r: 10, g: 20, b: 30, a: 50});
                        }
                    }
                    done();
                });
            });
        });

    });
});
