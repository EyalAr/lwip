// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.border arguments validation', () => {

    let batch;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid color object (1)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, {
                foo: 'bar',
                bar: 'foo'
            }).should.throwError();
        });
    });

    describe('invalid color object (2)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, {
                r: -5,
                g: -8,
                b: -1000
            }).should.throwError();
        });
    });

    describe('invalid color array (1)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, ['a', 'b']).should.throwError();
        });
    });

    describe('invalid color array (2)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, ['a', 'b', 'c']).should.throwError();
        });
    });

    describe('invalid color array (3)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, [0, -222, 202]).should.throwError();
        });
    });

    describe('invalid color array (4)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, [0, 222, -202]).should.throwError();
        });
    });

    describe('invalid color array (5)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, [1000, 100, 100]).should.throwError();
        });
    });

    describe('invalid color array (6)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, [100, 1000, 100]).should.throwError();
        });
    });

    describe('invalid color array (7)', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, [100, 100, 1000]).should.throwError();
        });
    });

    describe('invalid color string', () => {
        it('should throw an error', () => {
            batch.border.bind(batch, 5, 'foo').should.throwError();
        });
    });
});
