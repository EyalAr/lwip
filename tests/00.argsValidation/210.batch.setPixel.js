// methods should throw errors when arguments are invalid

const should = require("should"),
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.setPixel arguments validation', () => {

    let batch;
    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            batch = img.batch();
            done(err);
        });
    });

    describe('pixel exceeds dimensions', () => {

        describe('at the time of exec', () => {
            it('should return an error', done => {
                batch.setPixel.bind(batch, 999, 999, 'yellow').should.not.throwError();
                batch.exec(err => {
                    // there should be an error message
                    assert(!!err);
                    done();
                });
            });
        });

        describe('before exec', () => {
            it('should not return an error', done => {
                batch.resize(1000, 1000);
                batch.setPixel.bind(batch, 999, 999, 'yellow').should.not.throwError();
                batch.exec(err => {
                    // there should not be an error message
                    assert(!err);
                    done();
                });
            });
        });
    });
});
