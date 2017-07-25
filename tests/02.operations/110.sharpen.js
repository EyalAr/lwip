const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'sharpen';
let current;

describe('lwip.sharpen', () => {

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

    describe('amp=0', () => {
        const amp = 0;
        it('should succeed', done => {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=5.5', () => {
        const amp = 5.5;
        it('should succeed', done => {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=99', () => {
        const amp = 99;
        it('should succeed', done => {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=-5.5', () => {
        const amp = -5.5;
        it('should succeed', done => {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=-99', () => {
        const amp = -99;
        it('should succeed', done => {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

});
