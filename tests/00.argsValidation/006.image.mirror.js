// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.mirror arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid axis - g', function() {
        it('should throw an error', function() {
            image.mirror.bind(image, 'g', function() {}).should.throwError();
        });
    });
    describe('invalid axis - kjh', function() {
        it('should throw an error', function() {
            image.mirror.bind(image, 'kjh', function() {}).should.throwError();
        });
    });
});
