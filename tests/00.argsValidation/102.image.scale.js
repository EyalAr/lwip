// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.scale arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('invalid interpolation', () => {
        it('should throw an error', () => {
            image.scale.bind(image, 0.5, 0.5, 'foo', () => {}).should.throwError();
        });
    });
});
