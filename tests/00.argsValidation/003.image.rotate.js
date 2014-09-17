// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('image.rotate arguments validation', function() {

    var image;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('invalid color object (1)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, {
                foo: 'bar',
                bar: 'foo'
            }, function() {}).should.throwError();
        });
    });

    describe('invalid color object (2)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, {
                r: -5,
                g: -8,
                b: -1000
            }, function() {}).should.throwError();
        });
    });

    describe('invalid color array (1)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, ['a', 'b'], function() {}).should.throwError();
        });
    });

    describe('invalid color array (2)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, ['a', 'b', 'c'], function() {}).should.throwError();
        });
    });

    describe('invalid color array (3)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, [100, -100, 100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (4)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, [100, 100, -100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (5)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, [1000, 100, 100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (6)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, [100, 1000, 100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (7)', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, [100, 100, 1000], function() {}).should.throwError();
        });
    });

    describe('invalid color string', function() {
        it('should throw an error', function() {
            image.rotate.bind(image, 5, 'foo', function() {}).should.throwError();
        });
    });
});
