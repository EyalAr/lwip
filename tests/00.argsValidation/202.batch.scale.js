// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.scale arguments validation', () => {

    let batch;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid interpolation', () => {
        it('should throw an error', () => {
            batch.scale.bind(batch, 0.5, 0.5, 'foo').should.throwError();
        });
    });
});
