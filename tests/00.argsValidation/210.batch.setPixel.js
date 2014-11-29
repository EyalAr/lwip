// methods should throw errors when arguments are invalid

var should = require("should"),
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.setPixel arguments validation', function() {

    var batch;
    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch();
            done(err);
        });
    });

    describe('pixel exceeds dimensions', function() {

        describe('at the time of exec', function() {
            it('should return an error', function(done) {
                batch.setPixel.bind(batch, 999, 999, 'yellow').should.not.throwError();
                batch.exec(function(err) {
                    // there should be an error message
                    assert(!!err);
                    done();
                });
            });
        });

        describe('before exec', function() {
            it('should not return an error', function(done) {
                batch.resize(1000, 1000);
                batch.setPixel.bind(batch, 999, 999, 'yellow').should.not.throwError();
                batch.exec(function(err) {
                    // there should not be an error message
                    assert(!err);
                    done();
                });
            });
        });
    });
});
