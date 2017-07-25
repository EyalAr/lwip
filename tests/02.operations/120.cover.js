const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'cover';
let current;

describe('lwip.cover', () => {

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
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpg', {
            quality: 90
        }, done);
    });

    beforeEach(() => {
        current = [ basename ];
    });

    describe('800X300, unspecified interpolation', () => {
        it('image should have the correct size', done => {
            current.push('800X300','unspecified_inter');
            image.cover(800, 300, (err, im) => {
                if (err) return done(err);
                assert(im.width() === 800);
                assert(im.height() === 300);
                done();
            });
        });
    });

    describe('300X800, lanczos interpolation', () => {
        it('image should have the correct size', done => {
            current.push('300X800','lanczos');
            image.cover(300, 800, 'lanczos', (err, im) => {
                if (err) return done(err);
                assert(im.width() === 300);
                assert(im.height() === 800);
                done();
            });
        });
    });

});
