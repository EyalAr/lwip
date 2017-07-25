const join = require('path').join,
    should = require('should'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('simultaneous operations locks', () => {

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

    describe('image.resize lock', () => {
        it('should lock image', () => {
            image.resize.bind(image, 100, 100, () => {}).should.not.throwError();
            image.scale.bind(image, 0.5, 0.5, () => {}).should.throwError();
        });
    });

    describe('image.scale lock', () => {
        it('should lock image', () => {
            image.scale.bind(image, 0.5, 0.5, () => {}).should.not.throwError();
            image.rotate.bind(image, 10, () => {}).should.throwError();
        });
    });

    describe('image.rotate lock', () => {
        it('should lock image', () => {
            image.rotate.bind(image, 10, () => {}).should.not.throwError();
            image.crop.bind(image, 10, 10, () => {}).should.throwError();
        });
    });

    describe('image.crop lock', () => {
        it('should lock image', () => {
            image.crop.bind(image, 10, 10, () => {}).should.not.throwError();
            image.blur.bind(image, 1, () => {}).should.throwError();
        });
    });

    describe('image.blur lock', () => {
        it('should lock image', () => {
            image.blur.bind(image, 1, () => {}).should.not.throwError();
            image.sharpen.bind(image, 1, () => {}).should.throwError();
        });
    });

    describe('image.sharpen lock', () => {
        it('should lock image', () => {
            image.sharpen.bind(image, 1, () => {}).should.not.throwError();
            image.mirror.bind(image, 'x', () => {}).should.throwError();
        });
    });

    describe('image.mirror lock', () => {
        it('should lock image', () => {
            image.mirror.bind(image, 'x', () => {}).should.not.throwError();
            image.border.bind(image, 10, () => {}).should.throwError();
        });
    });

    describe('image.border lock', () => {
        it('should lock image', () => {
            image.border.bind(image, 10, () => {}).should.not.throwError();
            image.pad.bind(image, 10, 10, 10, 10, () => {}).should.throwError();
        });
    });

    describe('image.pad lock', () => {
        it('should lock image', () => {
            image.pad.bind(image, 10, 10, 10, 10, () => {}).should.not.throwError();
            image.saturate.bind(image, 1, () => {}).should.throwError();
        });
    });

    describe('image.saturate lock', () => {
        it('should lock image', () => {
            image.saturate.bind(image, 1, () => {}).should.not.throwError();
            image.lighten.bind(image, 1, () => {}).should.throwError();
        });
    });

    describe('image.lighten lock', () => {
        it('should lock image', () => {
            image.lighten.bind(image, 1, () => {}).should.not.throwError();
            image.darken.bind(image, 1, () => {}).should.throwError();
        });
    });

    describe('image.darken lock', () => {
        it('should lock image', () => {
            image.darken.bind(image, 1, () => {}).should.not.throwError();
            image.hue.bind(image, 100, () => {}).should.throwError();
        });
    });

    describe('image.hue lock', () => {
        it('should lock image', () => {
            image.hue.bind(image, 10, () => {}).should.not.throwError();
            image.fade.bind(image, 1, () => {}).should.throwError();
        });
    });

    describe('image.fade lock', () => {
        it('should lock image', () => {
            image.fade.bind(image, 1, () => {}).should.not.throwError();
            image.opacify.bind(image, () => {}).should.throwError();
        });
    });

    describe('image.opacify lock', () => {
        it('should lock image', () => {
            image.opacify.bind(image, () => {}).should.not.throwError();
            image.hslaAdjust.bind(image, 100, 1, 1, 1, () => {}).should.throwError();
        });
    });

    describe('image.hslaAdjust lock', () => {
        it('should lock image', () => {
            image.hslaAdjust.bind(image, 100, 1, 1, 0, () => {}).should.not.throwError();
            image.setPixel.bind(image, 0, 0, 'yellow', () => {}).should.throwError();
        });
    });

    describe('image.setPixel lock', () => {
        it('should lock image', () => {
            image.setPixel.bind(image, 0, 0, 'yellow', () => {}).should.not.throwError();
            image.paste.bind(image, 0, 0, tmpImage, () => {}).should.throwError();
        });
    });

    describe('image.paste lock', () => {
        it('should lock image', () => {
            image.paste.bind(image, 0, 0, tmpImage, () => {}).should.not.throwError();
            image.contain.bind(image, 100, 100, () => {}).should.throwError();
        });
    });

    describe('image.contain lock', () => {
        it('should lock image', () => {
            image.contain.bind(image, 100, 200, () => {}).should.not.throwError();
            image.cover.bind(image, 100, 100, () => {}).should.throwError();
        });
    });

    describe('image.cover lock', () => {
        it('should lock image', () => {
            image.cover.bind(image, 200, 300, () => {}).should.not.throwError();
            image.resize.bind(image, 100, 100, () => {}).should.throwError();
        });
    });

});
