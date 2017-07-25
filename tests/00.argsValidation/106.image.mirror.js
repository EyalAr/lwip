// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.mirror arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('invalid axis - g', () => {
        it('should throw an error', () => {
            image.mirror.bind(image, 'g', () => {}).should.throwError();
        });
    });
    describe('invalid axis - kjh', () => {
        it('should throw an error', () => {
            image.mirror.bind(image, 'kjh', () => {}).should.throwError();
        });
    });
});
