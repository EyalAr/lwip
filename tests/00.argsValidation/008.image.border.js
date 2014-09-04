// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.border arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid color object', function() {
        it('should throw an error', function() {
            image.border.bind(image, 5, {
                foo: 'bar',
                bar: 'foo'
            }, function() {}).should.throwError();
        });
    });
    describe('invalid color array (1)', function() {
        it('should throw an error', function() {
            image.border.bind(image, 5, ['a', 'b'], function() {}).should.throwError();
        });
    });
    describe('invalid color array (2)', function() {
        it('should throw an error', function() {
            image.border.bind(image, 5, ['a', 'b', 'c'], function() {}).should.throwError();
        });
    });
    describe('invalid color string', function() {
        it('should throw an error', function() {
            image.border.bind(image, 5, 'foo', function() {}).should.throwError();
        });
    });
});
