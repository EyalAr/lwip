var join = require('path').join,
    should = require('should'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('simultaneous operations locks', function() {

    var image;

    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('image.resize lock', function() {
        it('should throw an error', function() {
            image.resize(100, 100, function() {});
            image.scale.bind(image, 0.5, 0.5, function() {}).should.throwError();
        });
    });

    describe('image.scale lock', function() {
        it('should throw an error', function() {
            image.scale(0.5, 0.5, function() {});
            image.rotate.bind(image, 10, function() {}).should.throwError();
        });
    });

    describe('image.rotate lock', function() {
        it('should throw an error', function() {
            image.rotate(10, function() {});
            image.crop.bind(image, 10, 10, function() {}).should.throwError();
        });
    });

    describe('image.crop lock', function() {
        it('should throw an error', function() {
            image.crop(10, 10, function() {});
            image.blur.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.blur lock', function() {
        it('should throw an error', function() {
            image.blur(1, function() {});
            image.sharpen.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.sharpen lock', function() {
        it('should throw an error', function() {
            image.sharpen(1, function() {});
            image.mirror.bind(image, 'x', function() {}).should.throwError();
        });
    });

    describe('image.mirror lock', function() {
        it('should throw an error', function() {
            image.mirror('x', function() {});
            image.border.bind(image, 10, function() {}).should.throwError();
        });
    });

    describe('image.border lock', function() {
        it('should throw an error', function() {
            image.border(10, function() {});
            image.pad.bind(image, 10, 10, 10, 10, function() {}).should.throwError();
        });
    });

    describe('image.pad lock', function() {
        it('should throw an error', function() {
            image.pad(10, 10, 10, 10, function() {});
            image.saturate.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.saturate lock', function() {
        it('should throw an error', function() {
            image.saturate(1, function() {});
            image.lighten.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.lighten lock', function() {
        it('should throw an error', function() {
            image.lighten(1, function() {});
            image.darken.bind(image, 1, function() {}).should.throwError();
        });
    });

    describe('image.darken lock', function() {
        it('should throw an error', function() {
            image.darken(1, function() {});
            image.hue.bind(image, 100, function() {}).should.throwError();
        });
    });

    describe('image.hue lock', function() {
        it('should throw an error', function() {
            image.hue(10, function() {});
            image.hslAdjust.bind(image, 100, 1, 1, function() {}).should.throwError();
        });
    });

    describe('image.hslAdjust lock', function() {
        it('should throw an error', function() {
            image.hslAdjust(100, 1, 1, function() {});
            image.resize.bind(image, 100, 100, function() {}).should.throwError();
        });
    });

});
