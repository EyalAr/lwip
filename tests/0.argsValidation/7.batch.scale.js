// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.scale arguments validation', function() {

    var batch;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid interpolation', function() {
        it('should throw an error', function() {
            batch.scale.bind(batch, 0.5, 0.5, 'foo').should.throwError();
        });
    });
});
