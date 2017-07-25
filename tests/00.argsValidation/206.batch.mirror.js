// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.mirror arguments validation', () => {

    let batch;
    before(done => {
        lwip.open(imgs.gif.trans, (err, img) => {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid axis - g', () => {
        it('should throw an error', () => {
            batch.mirror.bind(batch, 'g').should.throwError();
        });
    });
    describe('invalid axis - kjh', () => {
        it('should throw an error', () => {
            batch.flip.bind(batch, 'kjh').should.throwError();
        });
    });
});
