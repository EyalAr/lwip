// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.mirror arguments validation', function() {

    var batch;
    before(function(done) {
        lwip.open(imgs.gif.trans, function(err, img) {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid axis - g', function() {
        it('should throw an error', function() {
            batch.mirror.bind(batch, 'g').should.throwError();
        });
    });
    describe('invalid axis - kjh', function() {
        it('should throw an error', function() {
            batch.flip.bind(batch, 'kjh').should.throwError();
        });
    });
});
