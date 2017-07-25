// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.writeFile arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('invalid encoding format', () => {
        it('should throw an error', () => {
            image.writeFile.bind(image, 'res.jpg', 'jjpeg', () => {}).should.throwError();
        });
    });

    describe('JPEG params', () => {

        describe('invalid quality', () => {
            it('should throw an error', () => {
                image.writeFile.bind(image, 'res.jpg', 'jpeg', {
                    quality: 'foo'
                }, () => {}).should.throwError();
            });
        });

    });

    describe('PNG params', () => {

        describe('invalid compression', () => {
            it('should throw an error', () => {
                image.writeFile.bind(image, 'res.jpg', 'png', {
                    compression: 'foo'
                }, () => {}).should.throwError();
            });
        });

        describe('invalid interlaced', () => {
            it('should throw an error', () => {
                image.writeFile.bind(image, 'res.jpg', 'png', {
                    interlaced: 'foo'
                }, () => {}).should.throwError();
            });
        });

        describe('invalid transparency', () => {
            it('should throw an error', () => {
                image.writeFile.bind(image, 'res.jpg', 'png', {
                    transparency: 'foo'
                }, () => {}).should.throwError();
            });
        });

    });

    describe('GIF params', () => {

        describe("invalid params", () => {

            describe('invalid colors (wrong type)', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        colors: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid colors (<2)', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        colors: 1
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid colors (>256)', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        colors: 257
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid interlaced', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        interlaced: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid transparency', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid threshold (wrong type)', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: true,
                        threshold: 'foo'
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid threshold (<0)', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: true,
                        threshold: -1
                    }, () => {}).should.throwError();
                });
            });

            describe('invalid threshold (>100)', () => {
                it('should throw an error', () => {
                    image.writeFile.bind(image, 'res.gif', 'gif', {
                        transparency: true,
                        threshold: 101
                    }, () => {}).should.throwError();
                });
            });

        });

    });

});
