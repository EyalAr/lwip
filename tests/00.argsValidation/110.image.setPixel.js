// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.setPixel arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe("pixel coordinates out of bounds", function(){
        it("should throw an error", function(done){
            image.setPixel.bind(image, 9999, 0, 'yellow', function(){}).should.throwError();
            image.setPixel.bind(image, 0, 9999, 'yellow', function(){}).should.throwError();
            image.setPixel.bind(image, 9999, 9999, 'yellow', function(){}).should.throwError();
            image.setPixel.bind(image, 0, 0, 'yellow', done).should.not.throwError(); // sanity check
        });
    });
});
