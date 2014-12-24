var join = require('path').join,
    should = require('should'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('simultaneous operations locks', function() {

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

    describe('image.resize lock', function() {
        it('should lock image', function() {
            image.resize.bind(image, 100, 100, function() {}).should.not.throwError();
            image.scale.bind(image, 0.5, 0.5, function() {}).should.throwError();
        });
    });

    describe('image.scale lock', function() {
        it('should lock image', function() {
            image.scale.bind(image, 0.5, 0.5, function() {}).should.not.throwError();
            image.rotate.bind(image, 10, function() {}).should.throwError();
        });
    });

    describe('image.rotate lock', function() {
        it('should lock image', function() {
            image.rotate.bind(image, 10, function() {}).should.not.throwError();
            image.crop.bind(image, 10, 10, function() {}).should.throwError();
        });
    });

    describe('image.crop lock', function() {
        it('should lock image', function() {
            image.crop.bind(image, 10, 10, function() {}).should.not.throwError();
            image.blur.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.blur lock', function() {
        it('should lock image', function() {
            image.blur.bind(image, 1, function() {}).should.not.throwError();
            image.sharpen.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.sharpen lock', function() {
        it('should lock image', function() {
            image.sharpen.bind(image, 1, function() {}).should.not.throwError();
            image.mirror.bind(image, 'x', function() {}).should.throwError();
        });
    });

    describe('image.mirror lock', function() {
        it('should lock image', function() {
            image.mirror.bind(image, 'x', function() {}).should.not.throwError();
            image.border.bind(image, 10, function() {}).should.throwError();
        });
    });

    describe('image.border lock', function() {
        it('should lock image', function() {
            image.border.bind(image, 10, function() {}).should.not.throwError();
            image.pad.bind(image, 10, 10, 10, 10, function() {}).should.throwError();
        });
    });

    describe('image.pad lock', function() {
        it('should lock image', function() {
            image.pad.bind(image, 10, 10, 10, 10, function() {}).should.not.throwError();
            image.saturate.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.saturate lock', function() {
        it('should lock image', function() {
            image.saturate.bind(image, 1, function() {}).should.not.throwError();
            image.lighten.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.lighten lock', function() {
        it('should lock image', function() {
            image.lighten.bind(image, 1, function() {}).should.not.throwError();
            image.darken.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.darken lock', function() {
        it('should lock image', function() {
            image.darken.bind(image, 1, function() {}).should.not.throwError();
            image.hue.bind(image, 100, function() {}).should.throwError();
        });
    });

    describe('image.hue lock', function() {
        it('should lock image', function() {
            image.hue.bind(image, 10, function() {}).should.not.throwError();
            image.fade.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.fade lock', function() {
        it('should lock image', function() {
            image.fade.bind(image, 1, function() {}).should.not.throwError();
            image.opacify.bind(image, function() {}).should.throwError();
        });
    });

    describe('image.opacify lock', function() {
        it('should lock image', function() {
            image.opacify.bind(image, function() {}).should.not.throwError();
            image.hslaAdjust.bind(image, 100, 1, 1, 1, function() {}).should.throwError();
        });
    });

    describe('image.hslaAdjust lock', function() {
        it('should lock image', function() {
            image.hslaAdjust.bind(image, 100, 1, 1, 0, function() {}).should.not.throwError();
            image.setPixel.bind(image, 0, 0, 'yellow', function() {}).should.throwError();
        });
    });

    describe('image.setPixel lock', function() {
        it('should lock image', function() {
            image.setPixel.bind(image, 0, 0, 'yellow', function() {}).should.not.throwError();
            image.paste.bind(image, 0, 0, tmpImage, function() {}).should.throwError();
        });
    });

    describe('image.paste lock', function() {
        it('should lock image', function() {
            image.paste.bind(image, 0, 0, tmpImage, function() {}).should.not.throwError();
            image.contain.bind(image, 100, 100, function() {}).should.throwError();
        });
    });

    describe('image.contain lock', function() {
        it('should lock image', function() {
            image.contain.bind(image, 100, 200, function() {}).should.not.throwError();
            image.cover.bind(image, 100, 100, function() {}).should.throwError();
        });
    });

    describe('image.cover lock', function() {
        it('should lock image', function() {
            image.cover.bind(image, 200, 300, function() {}).should.not.throwError();
            image.resize.bind(image, 100, 100, function() {}).should.throwError();
        });
    });

});
