const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'hue';
let current;

describe('lwip.hue', () => {

    let image;

    before(done => {
        mkdirp(tmpDir, done);
    });

    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    beforeEach(() => {
        current = [basename];
    });

    afterEach(done => {
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('shift=0', () => {
        const d = 0;
        it('should succeed', done => {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=50.5', () => {
        const d = 50.5;
        it('should succeed', done => {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=-50', () => {
        const d = -50;
        it('should succeed', done => {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=500', () => {
        const d = 500;
        it('should succeed', done => {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=-500', () => {
        const d = -500;
        it('should succeed', done => {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

});
