// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.writeFile arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid encoding format', function() {
        it('should throw an error', function() {
            image.writeFile.bind(image, 'res.jpg', 'jjpeg', function() {}).should.throwError();
        });
    });

    describe('JPEG params', function() {

        describe('invalid quality', function() {
            it('should throw an error', function() {
                image.writeFile.bind(image, 'jpeg', {
                    quality: 'foo'
                }, function() {}).should.throwError();
            });
        });

    });

});
