// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.getPixel arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('coordinates exceeding image dimensions', () => {
        it('should throw an error', () => {
            image.getPixel.bind(image, 99999, 0).should.throwError();
            image.getPixel.bind(image, 0, 99999).should.throwError();
            image.getPixel.bind(image, 99999, 99999).should.throwError();
        });
    });

});
