// methods should throw errors when arguments are invalid

const should = require("should"),
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.paste arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('invalid pasted image (1)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, {}, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (2)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, null, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (3)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (4)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, () => {}, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (5)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, undefined, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (6)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, 0, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (7)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, "foo", () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (8)', () => {
        it('should throw an error', () => {
            image.paste.bind(image, 0, 0, [1, 2, 3], () => {}).should.throwError();
        });
    });

    describe('pasted image exceeds dimensions', () => {

        const Image = require('../../lib/Image');

        let clone;
        before(done => {
            image.clone((err, img) => {
                clone = img;
                done(err);
            });
        });

        it('should throw an error', () => {
            assert(clone instanceof Image);
            image.paste.bind(image, 10, 10, clone, () => {}).should.throwError();
        });
    });
});
