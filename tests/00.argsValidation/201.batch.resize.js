// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.resize arguments validation', () => {

    let batch;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid interpolation', () => {
        it('should throw an error', () => {
            batch.resize.bind(batch, 100, 100, 'foo').should.throwError();
        });
    });
});
