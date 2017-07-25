// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.toBuffer arguments validation', () => {

    let image;
    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('invalid encoding format', () => {
        it('should throw an error', () => {
            image.toBuffer.bind(image, 'jjpeg', () => {}).should.throwError();
        });
    });

    describe('JPEG params', () => {

        describe('invalid quality', () => {
            it('should throw an error', () => {
                image.toBuffer.bind(image, 'jpeg', {
                    quality: 'foo'
                }, () => {}).should.throwError();
            });
        });

    });

    describe('PNG params', () => {

        describe("valid params", () => {

            describe('defaults', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', done).should.not.throwError();
                });
            });

            describe('none, false, true', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: false,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('fast, false, true', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: false,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('high, false, true', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: false,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('none, true, true', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: true,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('fast, true, true', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: true,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('high, true, true', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: true,
                        transparency: true
                    }, done).should.not.throwError();
                });
            });

            describe('none, false, false', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: false,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('fast, false, false', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: false,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('high, false, false', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: false,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('none, true, false', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: true,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('fast, true, false', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: true,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('high, true, false', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: true,
                        transparency: false
                    }, done).should.not.throwError();
                });
            });

            describe('none, false, auto', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: false,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('fast, false, auto', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: false,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('high, false, auto', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: false,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('none, true, auto', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'none',
                        interlaced: true,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('fast, true, auto', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'fast',
                        interlaced: true,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

            describe('high, true, auto', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'high',
                        interlaced: true,
                        transparency: 'auto'
                    }, done).should.not.throwError();
                });
            });

        });

        describe("invalid params", () => {

            describe('invalid compression', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'png', {
                        compression: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid interlaced', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'png', {
                        interlaced: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid transparency', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'png', {
                        transparency: 'foo'
                    }, () => {}).should.throwError();
                });
            });

        });

    });

    describe('GIF params', () => {

        describe("valid params", () => {

            describe('defaults', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'gif', done).should.not.throwError();
                });
            });

            describe('120, false, false, 50', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 120,
                        interlaced: false,
                        transparency: false,
                        threshold: 50
                    }, done).should.not.throwError();
                });
            });

            describe('256, true, false, 50', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 256,
                        interlaced: true,
                        transparency: false,
                        threshold: 50
                    }, done).should.not.throwError();
                });
            });

            describe('2, false, true, 0', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 2,
                        interlaced: false,
                        transparency: true,
                        threshold: 0
                    }, done).should.not.throwError();
                });
            });

            describe('120, true, true, 100', () => {
                it('should succeed', done => {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 120,
                        interlaced: true,
                        transparency: true,
                        threshold: 100
                    }, done).should.not.throwError();
                });
            });

        });

        describe("invalid params", () => {

            describe('invalid colors (wrong type)', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid colors (<2)', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 1
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid colors (>256)', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        colors: 257
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid interlaced', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        interlaced: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid transparency', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid threshold (wrong type)', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: true,
                        threshold: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid threshold (<0)', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: true,
                        threshold: -1
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid threshold (>100)', () => {
                it('should throw an error', () => {
                    image.toBuffer.bind(image, 'gif', {
                        transparency: true,
                        threshold: 101
                    }, () => {}).should.throwError();
                });
            });

        });

    });

});
