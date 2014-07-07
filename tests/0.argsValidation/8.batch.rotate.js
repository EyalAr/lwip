// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.rotate arguments validation', function() {

    var batch;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid color object', function() {
        it('should throw an error', function() {
            batch.rotate.bind(batch, 5, {
                foo: 'bar',
                bar: 'foo'
            }).should.throwError();
        });
    });
    describe('invalid color array (1)', function() {
        it('should throw an error', function() {
            batch.rotate.bind(batch, 5, ['a', 'b']).should.throwError();
        });
    });
    describe('invalid color array (2)', function() {
        it('should throw an error', function() {
            batch.rotate.bind(batch, 5, ['a', 'b', 'c']).should.throwError();
        });
    });
    describe('invalid color string', function() {
        it('should throw an error', function() {
            batch.rotate.bind(batch, 5, 'foo').should.throwError();
        });
    });
});
