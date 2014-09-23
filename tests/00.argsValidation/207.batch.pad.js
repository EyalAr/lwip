// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.pad arguments validation', function() {

    var batch;
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid color object (1)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, {
                foo: 'bar',
                bar: 'foo'
            }).should.throwError();
        });
    });

    describe('invalid color object (2)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, {
                r: -5,
                g: -8,
                b: -1000
            }).should.throwError();
        });
    });

    describe('invalid color array (1)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, ['a', 'b']).should.throwError();
        });
    });

    describe('invalid color array (2)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, ['a', 'b', 'c']).should.throwError();
        });
    });

    describe('invalid color array (3)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, [0, -222, 202]).should.throwError();
        });
    });

    describe('invalid color array (4)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, [0, 222, -202]).should.throwError();
        });
    });

    describe('invalid color array (5)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, [1000, 100, 100]).should.throwError();
        });
    });

    describe('invalid color array (6)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, [100, 1000, 100]).should.throwError();
        });
    });

    describe('invalid color array (7)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, [100, 100, 1000]).should.throwError();
        });
    });

    describe('invalid color array (8)', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, [100, 100, 100, 1000]).should.throwError();
        });
    });

    describe('invalid color string', function() {
        it('should throw an error', function() {
            batch.pad.bind(batch, 5, 5, 5, 5, 'foo').should.throwError();
        });
    });
});
