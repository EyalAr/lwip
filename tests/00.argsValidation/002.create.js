// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.create arguments validation', () => {

    describe('invalid color object (1)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 500, 500, {
                foo: 'bar',
                bar: 'foo'
            }, () => {}).should.throwError();
        });
    });

    describe('invalid color object (2)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 5000, 5000, {
                r: -5,
                g: -8,
                b: -1000
            }, () => {}).should.throwError();
        });
    });

    describe('invalid color array (1)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 5, 5, ['a', 'b'], () => {}).should.throwError();
        });
    });

    describe('invalid color array (2)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 11, 43, ['a', 'b', 'c'], () => {}).should.throwError();
        });
    });

    describe('invalid color array (3)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 909, 1, [100, -100, 100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (4)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 1, 1, [100, 100, -100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (5)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 100, 50, [1000, 100, 100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (6)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 100, 50, [100, 1000, 100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (7)', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 100, 50, [100, 100, 1000], () => {}).should.throwError();
        });
    });

    describe('invalid color string', () => {
        it('should throw an error', () => {
            lwip.create.bind(lwip, 100, 50, 'foo', () => {}).should.throwError();
        });
    });
});
