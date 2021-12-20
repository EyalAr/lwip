const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
const basename = 'border';
let current;

describe('lwip.border', () => {

    let image;

    before(() => {
        return mkdirp(tmpDir);
    });

    beforeEach(() => {
        current = [basename];
    });

    beforeEach(done => {
        lwip.open(imgs.png.trans, (err, img) => {
            image = img;
            done(err);
        });
    });

    afterEach(done => {
        image.writeFile(join(tmpDir, current.join('_') + '.gif'), 'gif', {
            colors: 256
        }, done);
    });

    describe('19 pixels, green', () => {
        const width = 500 + 19 * 2,
            height = 333 + 19 * 2;
        it('image should have the correct size', done => {
            current.push('19px_green');
            image.border(19, 'green', (err, im) => {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

    describe('9 pixels, [100,200,25]', () => {
        const width = 500 + 9 * 2,
            height = 333 + 9 * 2;
        it('image should have the correct size', done => {
            current.push('9px_[100,200,25]');
            image.border(9, [100, 200, 25], (err, im) => {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

    describe('4 pixels, {r:100,g:200,b:25}', () => {
        const width = 500 + 4 * 2,
            height = 333 + 4 * 2;
        it('image should have the correct size', done => {
            current.push('4px_r100,g200,b25');
            image.border(4, {
                r: 100,
                g: 200,
                b: 25
            }, (err, im) => {
                if (err) return done(err);
                assert(im.width() === width);
                assert(im.height() === height);
                done();
            });
        });
    });

});
