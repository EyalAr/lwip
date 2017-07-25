const join = require('path').join,
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

const width = 500,
    height = 333;

let image;
before(done => {
    lwip.open(imgs.jpg.rgb, (err, img) => {
        image = img;
        done(err);
    });
});


describe('lwip.width', () => {
    it('should return correct width', () => {
        assert(image.width() === width);
    });
});

describe('lwip.height', () => {
    it('should return correct height', () => {
        assert(image.height() === height);
    });
});

describe('lwip.size', () => {
    it('should return correct size', () => {
        assert(image.size().width === width);
        assert(image.size().height === height);
    });
});

describe('lwip.getPixel', () => {
    it('should return correct color at 0,0', () => {
        const color = image.getPixel(0, 0);
        assert(color.r === 48);
        assert(color.g === 86);
        assert(color.b === 151);
        assert(color.a === 100);
    });

    it('should return correct color at 418, 242', () => {
        const color = image.getPixel(418, 242);
        assert(color.r === 208);
        assert(color.g === 228);
        assert(color.b === 237);
        assert(color.a === 100);
    });

    it('should return correct color at 499, 332', () => {
        const color = image.getPixel(499, 332);
        assert(color.r === 31);
        assert(color.g === 26 || color.g === 27);
        assert(color.b === 0);
        assert(color.a === 100);
    });
});

describe('lwip.clone', () => {
    it('should return a new image object', done => {
        image.clone((err, clonedImage) => {
            if (err) return done(err);
            clonedImage.resize(100, 100, err => {
                if (err) return done(err);
                assert(image.width() === width);
                assert(image.height() === height);
                assert(clonedImage.width() === 100);
                assert(clonedImage.height() === 100);
                done();
            });
        });
    });
});

describe('lwip.extract', () => {
    it('should return a new image object', done => {
        image.extract(100, 120, 150, 140, (err, exImage) => {
            if (err) return done(err);
            assert(image.width() === width);
            assert(image.height() === height);
            assert(exImage.width() === 150 - 100 + 1);
            assert(exImage.height() === 140 - 120 + 1);
            done();
        });
    });
});

describe('lwip.getMetadata', () => {
    it('should return correct metadata', done => {
        lwip.open(imgs.png.hasMetadata, (err, img) => {
            assert(img.getMetadata() === "Lorem ipsum dolor sit amet");
            done();
        });
    });

    it('should return null if no metadata found', done => {
        lwip.open(imgs.png.noMetadata, (err, img) => {
            assert(img.getMetadata() === null);
            done();
        });
    });
});
