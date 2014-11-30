// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.getPixel arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('coordinates exceeding image dimensions', function() {
        it('should throw an error', function() {
            image.getPixel.bind(image, 99999, 0).should.throwError();
            image.getPixel.bind(image, 0, 99999).should.throwError();
            image.getPixel.bind(image, 99999, 99999).should.throwError();
        });
    });

});
