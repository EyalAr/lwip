// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.resize arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('invalid interpolation', () => {
        it('should throw an error', () => {
            image.resize.bind(image, 100, 100, 'foo', () => {}).should.throwError();
        });
    });
});
