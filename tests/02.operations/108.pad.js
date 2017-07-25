const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'pad';
    current = [basename];

describe('lwip.pad', () => {

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
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('left-1, top-3, right-5, bottom-8, color-green', () => {
        const width = 500 + 1 + 5,
            height = 333 + 3 + 8;
        it('image should have the correct size', done => {
            current.push('l1_t3_r5_b8_green');
            image.pad(1, 3, 5, 8, 'green', (err, im) => {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

    describe('left-10, top-30, right-50, bottom-80, color-[120,50,200]', () => {
        const width = 500 + 10 + 50,
            height = 333 + 30 + 80;
        it('image should have the correct size', done => {
            current.push('l10_t30_r50_b80_[120,50,200]');
            image.pad(10, 30, 50, 80, [120, 50, 200], (err, im) => {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

    describe('left-10, top-30, right-50, bottom-80, color-[20,150,20,80]', () => {
        const width = 500 + 10 + 50,
            height = 333 + 30 + 80;
        it('image should have the correct size', done => {
            current.push('l10_t30_r50_b80_[20,150,20,80]');
            image.pad(10, 30, 50, 80, [20, 150, 20, 80], (err, im) => {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

    describe('left-5, top-15, right-25, bottom-40, color-{r:120,g:50,b:200}', () => {
        const width = 500 + 5 + 25,
            height = 333 + 15 + 40;
        it('image should have the correct size', done => {
            current.push('l1_t15_r25_b40_red120_grn50_blu200');
            image.pad(5, 15, 25, 40, {
                r: 120,
                g: 50,
                b: 200
            }, (err, im) => {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

});
