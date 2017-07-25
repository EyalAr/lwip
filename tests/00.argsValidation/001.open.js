// methods should throw errors when arguments are invalid

const should = require("should"),
    fs = require('fs'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.open arguments validation', () => {

    describe('invalid source', () => {

        describe('with type', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, 123, 'jpg', () => {}).should.throwError();
            });
        });
        describe('without type', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, {}, () => {}).should.throwError();
            });
        });

    });

    describe('path', () => {

        describe('without extension and no type', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, imgs.jpg.noex, () => {}).should.throwError();
            });
        });
        describe('with unknown extension and no type', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, imgs.inv, () => {}).should.throwError();
            });
        });
        describe('with invalid type', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, imgs.jpg.rgb, 'jjpg', () => {}).should.throwError();
            });
        });
        describe('with invalid type (raw buffer properties)', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, imgs.jpg.rgb, {width: 120, height: 120}, () => {}).should.throwError();
            });
        });

    });

    describe('buffer', () => {

        let buffer;
        before(done => {
            fs.readFile(imgs.jpg.rgb, (err, buff) => {
                buffer = buff;
                done();
            });
        });

        describe('without type', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, () => {}).should.throwError();
            });
        });
        describe('with invalid type', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, 'jjpg', () => {}).should.throwError();
            });
        });

    });

    describe('pixelbuffer', () => {

        let buffer;
        before(done => {
            buffer = new Buffer(120 * 120);
            done();
        });

        describe('without raw buffer properties', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, () => {}).should.throwError();
            });
        });

        describe('without width', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { height: 120 }, () => {}).should.throwError();
            });
        });

        describe('without height', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: 120 }, () => {}).should.throwError();
            });
        });

        describe('without width and height', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { }, () => {}).should.throwError();
            });
        });

        describe('with non numeric width', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: "lorem", height: 120 }, () => {}).should.throwError();
            });
        });

        describe('with non numeric height', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: 120, height: "lorem" }, () => {}).should.throwError();
            });
        });

        describe('with non numeric width and height', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: "lorem", height: "ipsum" }, () => {}).should.throwError();
            });
        });

        describe('with negative width', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: -120, height: 120 }, () => {}).should.throwError();
            });
        });

        describe('with negative height', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: 120, height: -120 }, () => {}).should.throwError();
            });
        });

        describe('with negative width and height', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: -120, height: -120 }, () => {}).should.throwError();
            });
        });

        describe('with incorrect width and height', () => {
            it('should throw an error', () => {
                lwip.open.bind(lwip, buffer, { width: 123, height: 321 }, () => {}).should.throwError();
            });
        });

        describe('with correct width and height for 1 channel', () => {
            it('should succeed', () => {
                lwip.open.bind(lwip, buffer, { width: 120, height: 120 }, () => {}).should.not.throw();
            });
        });

        describe('with correct width and height for 2 channels', () => {
            const newBuffer = new Buffer(120 * 120 * 2);
            it('should succeed', () => {
                lwip.open.bind(lwip, newBuffer, { width: 120, height: 120 }, () => {}).should.not.throw();
            });
        });

        describe('with correct width and height for 3 channels', () => {
            const newBuffer = new Buffer(120 * 120 * 3);
            it('should succeed', () => {
                lwip.open.bind(lwip, newBuffer, { width: 120, height: 120 }, () => {}).should.not.throw();
            });
        });

        describe('with correct width and height for 4 channels', () => {
            const newBuffer = new Buffer(120 * 120 * 4);
            it('should succeed', () => {
                lwip.open.bind(lwip, newBuffer, { width: 120, height: 120 }, () => {}).should.not.throw();
            });
        });

    });

});
