// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.create arguments validation', function() {

    describe('invalid color object (1)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 500, 500, {
                foo: 'bar',
                bar: 'foo'
            }, function() {}).should.throwError();
        });
    });

    describe('invalid color object (2)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 5000, 5000, {
                r: -5,
                g: -8,
                b: -1000
            }, function() {}).should.throwError();
        });
    });

    describe('invalid color array (1)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 5, 5, ['a', 'b'], function() {}).should.throwError();
        });
    });

    describe('invalid color array (2)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 11, 43, ['a', 'b', 'c'], function() {}).should.throwError();
        });
    });

    describe('invalid color array (3)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 909, 1, [100, -100, 100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (4)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 1, 1, [100, 100, -100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (5)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 100, 50, [1000, 100, 100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (6)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 100, 50, [100, 1000, 100], function() {}).should.throwError();
        });
    });

    describe('invalid color array (7)', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 100, 50, [100, 100, 1000], function() {}).should.throwError();
        });
    });

    describe('invalid color string', function() {
        it('should throw an error', function() {
            lwip.create.bind(lwip, 100, 50, 'foo', function() {}).should.throwError();
        });
    });
});
