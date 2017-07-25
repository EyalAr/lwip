// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.setPixel arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe("pixel coordinates out of bounds", () => {
        it("should throw an error", done => {
            image.setPixel.bind(image, 9999, 0, 'yellow', () => {}).should.throwError();
            image.setPixel.bind(image, 0, 9999, 'yellow', () => {}).should.throwError();
            image.setPixel.bind(image, 9999, 9999, 'yellow', () => {}).should.throwError();
            image.setPixel.bind(image, 0, 0, 'yellow', done).should.not.throwError(); // sanity check
        });
    });
});
