// methods should throw errors when arguments are invalid

const should = require("should"),
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.paste arguments validation', () => {

    let batch;
    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid pasted image (1)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, {}, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (2)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, null, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (3)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (4)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, () => {}, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (5)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, undefined, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (6)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, 0, () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (7)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, "foo", () => {}).should.throwError();
        });
    });

    describe('invalid pasted image (8)', () => {
        it('should throw an error', () => {
            batch.paste.bind(batch, 0, 0, [1, 2, 3], () => {}).should.throwError();
        });
    });

    describe('pasted image exceeds dimensions', () => {

        const Image = require('../../lib/Image');
        console.log(Image);

        let pst;
        beforeEach(done => {
            lwip.open(imgs.png.rgb, (err, img) => {
                pst = img;
                done(err);
            });
        });

        describe('at the time of exec', () => {
            it('should return an error', done => {
                assert(pst instanceof Image);
                batch.paste.bind(batch, 10, 10, pst).should.not.throwError();
                batch.exec(err => {
                    // there should be an error message
                    assert(!!err);
                    done();
                });
            });
        });

        describe('before exec', () => {
            it('should not return an error', done => {
                assert(pst instanceof Image);
                batch.paste.bind(batch, 10, 10, pst).should.not.throwError();
                pst.scale(0.1, (err, pst) => {
                    batch.exec(err => {
                        // there should not be an error message
                        assert(!err);
                        done();
                    });
                });
            });
        });
    });
});
