const join = require('path').join,
    should = require('should'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('failed ops lock release', () => {

    let image, tmpImage;

    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    before(done => {
        lwip.create(10, 10, (err, img) => {
            tmpImage = img;
            done(err);
        });
    });

    describe('image.resize release', () => {
        it('should release image lock', () => {
            image.resize.bind(image, 'foo', 'bar', () => {}).should.throwError();
            image.scale.bind(image, 0.5, 0.5, () => {}).should.not.throwError();
        });
    });

    describe('image.scale release', () => {
        it('should release image lock', () => {
            image.scale.bind(image, 'foo', 'bar', () => {}).should.throwError();
            image.rotate.bind(image, 10, () => {}).should.not.throwError();
        });
    });

    describe('image.rotate release', () => {
        it('should release image lock', () => {
            image.rotate.bind(image, 'foo', () => {}).should.throwError();
            image.crop.bind(image, 10, 10, () => {}).should.not.throwError();
        });
    });

    describe('image.crop release', () => {
        it('should release image lock', () => {
            image.crop.bind(image, 'foo', 'bar', () => {}).should.throwError();
            image.blur.bind(image, 1, () => {}).should.not.throwError();
        });
    });

    describe('image.blur release', () => {
        it('should release image lock', () => {
            image.blur.bind(image, 'foo', () => {}).should.throwError();
            image.sharpen.bind(image, 1, () => {}).should.not.throwError();
        });
    });

    describe('image.sharpen release', () => {
        it('should release image lock', () => {
            image.sharpen.bind(image, 'foo', () => {}).should.throwError();
            image.mirror.bind(image, 'x', () => {}).should.not.throwError();
        });
    });

    describe('image.mirror release', () => {
        it('should release image lock', () => {
            image.mirror.bind(image, 'foo', () => {}).should.throwError();
            image.border.bind(image, 10, () => {}).should.not.throwError();
        });
    });

    describe('image.border release', () => {
        it('should release image lock', () => {
            image.border.bind(image, 'foo', () => {}).should.throwError();
            image.pad.bind(image, 10, 10, 10, 10, () => {}).should.not.throwError();
        });
    });

    describe('image.pad release', () => {
        it('should release image lock', () => {
            image.pad.bind(image, 'foo', 'bar', 'foo', 'bar', () => {}).should.throwError();
            image.saturate.bind(image, 1, () => {}).should.not.throwError();
        });
    });

    describe('image.saturate release', () => {
        it('should release image lock', () => {
            image.saturate.bind(image, 'foo', () => {}).should.throwError();
            image.lighten.bind(image, 1, () => {}).should.not.throwError();
        });
    });

    describe('image.lighten release', () => {
        it('should release image lock', () => {
            image.lighten.bind(image, 'foo', () => {}).should.throwError();
            image.darken.bind(image, 1, () => {}).should.not.throwError();
        });
    });

    describe('image.darken release', () => {
        it('should release image lock', () => {
            image.darken.bind(image, 'foo', () => {}).should.throwError();
            image.hue.bind(image, 100, () => {}).should.not.throwError();
        });
    });

    describe('image.hue release', () => {
        it('should release image lock', () => {
            image.hue.bind(image, 'foo', () => {}).should.throwError();
            image.fade.bind(image, 1, () => {}).should.not.throwError();
        });
    });

    describe('image.fade release', () => {
        it('should release image lock', () => {
            image.fade.bind(image, 'foo', () => {}).should.throwError();
            image.opacify.bind(image, () => {}).should.not.throwError();
        });
    });

    describe('image.opacify release', () => {
        it('should release image lock', () => {
            image.opacify.bind(image, 'foo').should.throwError();
            image.hslaAdjust.bind(image, 100, 1, 1, 0, () => {}).should.not.throwError();
        });
    });

    describe('image.hslaAdjust release', () => {
        it('should release image lock', () => {
            image.hslaAdjust.bind(image, 'foo', 'foo', 'foo', 'foo', () => {}).should.throwError();
            image.setPixel.bind(image, 0, 0, 'yellow', () => {}).should.not.throwError();
        });
    });

    describe('image.setPixel release', () => {
        it('should release image lock', () => {
            image.setPixel.bind(image, 'foo', 'foo', 'foo', () => {}).should.throwError();
            image.paste.bind(image, 0, 0, tmpImage, () => {}).should.not.throwError();
        });
    });

    describe('image.paste release', () => {
        it('should release image lock', () => {
            image.paste.bind(image, 'foo', 'foo', 'foo', () => {}).should.throwError();
            image.contain.bind(image, 100, 200, () => {}).should.not.throwError();
        });
    });

    describe('image.contain release', () => {
        it('should release image lock', () => {
            image.contain.bind(image, 'foo', 'foo', 'foo', () => {}).should.throwError();
            image.cover.bind(image, 100, 100, () => {}).should.not.throwError();
        });
    });

    describe('image.cover release', () => {
        it('should release image lock', () => {
            image.cover.bind(image, 'foo', 'foo', () => {}).should.throwError();
            image.resize.bind(image, 100, 100, () => {}).should.not.throwError();
        });
    });

});
