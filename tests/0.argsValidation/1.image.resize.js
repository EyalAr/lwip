// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.resize arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid interpolation', function() {
        it('should throw an error', function() {
            image.resize.bind(image, 100, 100, 'foo', function() {}).should.throwError();
        });
    });
});
