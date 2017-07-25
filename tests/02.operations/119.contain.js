const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'contain';
let current;

describe('lwip.contain', () => {

    let image;

    before(done => {
        mkdirp(tmpDir, done);
    });

    beforeEach(done => {
        lwip.open(imgs.png.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    afterEach(done => {
        image.writeFile(join(tmpDir, current.join('_') + '.gif'), 'gif', {
            colors: 256,
            interlaced: true
        }, done);
    });

    beforeEach(() => {
        current = [ basename ];
    });

    describe('800X100, no color, unspecified interpolation', () => {
        it('image should have the correct size', done => {
            current.push('800X100','no_color','unspecified_inter');
            image.contain(800, 100, (err, im) => {
                if (err) return done(err);
                assert(im.width() === 800);
                assert(im.height() === 100);
                done();
            });
        });
    });

    describe('100X800, no color, lanczos interpolation', () => {
        it('image should have the correct size', done => {
            current.push('100X800','no_color','lanczos');
            image.contain(100, 800, 'lanczos', (err, im) => {
                if (err) return done(err);
                assert(im.width() === 100);
                assert(im.height() === 800);
                done();
            });
        });
    });

    describe('100X400, gray, unspecified interpolation', () => {
        it('image should have the correct size', done => {
            current.push('100X400','gray','unspecified_inter');
            image.contain(100, 400, 'gray', (err, im) => {
                if (err) return done(err);
                assert(im.width() === 100);
                assert(im.height() === 400);
                done();
            });
        });
    });

    describe('400X100, gray, lanczos interpolation', () => {
        it('image should have the correct size', done => {
            current.push('400X100','gray','lanczos');
            image.contain(400, 100, 'gray', 'lanczos', (err, im) => {
                if (err) return done(err);
                assert(im.width() === 400);
                assert(im.height() === 100);
                done();
            });
        });
    });

});
