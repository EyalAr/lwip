var join = require('path').join,
    should = require('should'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('failed ops lock release', function() {

    var image, tmpImage;

    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    before(function(done){
        lwip.create(10, 10, function(err, img){
            tmpImage = img;
            done(err);
        });
    });

    describe('image.resize release', function() {
        it('should release image lock', function() {
            image.resize.bind(image, 'foo', 'bar', function() {}).should.throwError();
            image.scale.bind(image, 0.5, 0.5, function() {}).should.not.throwError();
        });
    });

    describe('image.scale release', function() {
        it('should release image lock', function() {
            image.scale.bind(image, 'foo', 'bar', function() {}).should.throwError();
            image.rotate.bind(image, 10, function() {}).should.not.throwError();
        });
    });

    describe('image.rotate release', function() {
        it('should release image lock', function() {
            image.rotate.bind(image, 'foo', function() {}).should.throwError();
            image.crop.bind(image, 10, 10, function() {}).should.not.throwError();
        });
    });

    describe('image.crop release', function() {
        it('should release image lock', function() {
            image.crop.bind(image, 'foo', 'bar', function() {}).should.throwError();
            image.blur.bind(image, 1, function() {}).should.not.throwError();
        });
    });

    describe('image.blur release', function() {
        it('should release image lock', function() {
            image.blur.bind(image, 'foo', function() {}).should.throwError();
            image.sharpen.bind(image, 1, function() {}).should.not.throwError();
        });
    });

    describe('image.sharpen release', function() {
        it('should release image lock', function() {
            image.sharpen.bind(image, 'foo', function() {}).should.throwError();
            image.mirror.bind(image, 'x', function() {}).should.not.throwError();
        });
    });

    describe('image.mirror release', function() {
        it('should release image lock', function() {
            image.mirror.bind(image, 'foo', function() {}).should.throwError();
            image.border.bind(image, 10, function() {}).should.not.throwError();
        });
    });

    describe('image.border release', function() {
        it('should release image lock', function() {
            image.border.bind(image, 'foo', function() {}).should.throwError();
            image.pad.bind(image, 10, 10, 10, 10, function() {}).should.not.throwError();
        });
    });

    describe('image.pad release', function() {
        it('should release image lock', function() {
            image.pad.bind(image, 'foo', 'bar', 'foo', 'bar', function() {}).should.throwError();
            image.saturate.bind(image, 1, function() {}).should.not.throwError();
        });
    });

    describe('image.saturate release', function() {
        it('should release image lock', function() {
            image.saturate.bind(image, 'foo', function() {}).should.throwError();
            image.lighten.bind(image, 1, function() {}).should.not.throwError();
        });
    });

    describe('image.lighten release', function() {
        it('should release image lock', function() {
            image.lighten.bind(image, 'foo', function() {}).should.throwError();
            image.darken.bind(image, 1, function() {}).should.not.throwError();
        });
    });

    describe('image.darken release', function() {
        it('should release image lock', function() {
            image.darken.bind(image, 'foo', function() {}).should.throwError();
            image.hue.bind(image, 100, function() {}).should.not.throwError();
        });
    });

    describe('image.hue release', function() {
        it('should release image lock', function() {
            image.hue.bind(image, 'foo', function() {}).should.throwError();
            image.fade.bind(image, 1, function() {}).should.not.throwError();
        });
    });

    describe('image.fade release', function() {
        it('should release image lock', function() {
            image.fade.bind(image, 'foo', function() {}).should.throwError();
            image.opacify.bind(image, function() {}).should.not.throwError();
        });
    });

    describe('image.opacify release', function() {
        it('should release image lock', function() {
            image.opacify.bind(image, 'foo').should.throwError();
            image.hslaAdjust.bind(image, 100, 1, 1, 0, function() {}).should.not.throwError();
        });
    });

    describe('image.hslaAdjust release', function() {
        it('should release image lock', function() {
            image.hslaAdjust.bind(image, 'foo', 'foo', 'foo', 'foo', function() {}).should.throwError();
            image.setPixel.bind(image, 0, 0, 'yellow', function() {}).should.not.throwError();
        });
    });

    describe('image.setPixel release', function() {
        it('should release image lock', function() {
            image.setPixel.bind(image, 'foo', 'foo', 'foo', function() {}).should.throwError();
            image.paste.bind(image, 0, 0, tmpImage, function() {}).should.not.throwError();
        });
    });

    describe('image.paste release', function() {
        it('should release image lock', function() {
            image.paste.bind(image, 'foo', 'foo', 'foo', function() {}).should.throwError();
            image.contain.bind(image, 100, 200, function() {}).should.not.throwError();
        });
    });

    describe('image.contain release', function() {
        it('should release image lock', function() {
            image.contain.bind(image, 'foo', 'foo', 'foo', function() {}).should.throwError();
            image.cover.bind(image, 100, 100, function() {}).should.not.throwError();
        });
    });

    describe('image.cover release', function() {
        it('should release image lock', function() {
            image.cover.bind(image, 'foo', 'foo', function() {}).should.throwError();
            image.resize.bind(image, 100, 100, function() {}).should.not.throwError();
        });
    });

});
