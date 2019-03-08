// methods should throw errors when arguments are invalid

const lwip = require('../../'),
    imgs = require('../imgs');

describe('image.rotate arguments validation', () => {

    let image;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('invalid color object (1)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, {
                foo: 'bar',
                bar: 'foo'
            }, () => {}).should.throwError();
        });
    });

    describe('invalid color object (2)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, {
                r: -5,
                g: -8,
                b: -1000
            }, () => {}).should.throwError();
        });
    });

    describe('invalid color array (1)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, ['a', 'b'], () => {}).should.throwError();
        });
    });

    describe('invalid color array (2)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, ['a', 'b', 'c'], () => {}).should.throwError();
        });
    });

    describe('invalid color array (3)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, [100, -100, 100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (4)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, [100, 100, -100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (5)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, [1000, 100, 100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (6)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, [100, 1000, 100], () => {}).should.throwError();
        });
    });

    describe('invalid color array (7)', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, [100, 100, 1000], () => {}).should.throwError();
        });
    });

    describe('invalid color string', () => {
        it('should throw an error', () => {
            image.rotate.bind(image, 5, 'foo', () => {}).should.throwError();
        });
    });
});
