const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'hslaAdjust';
let current;

describe('lwip.hslaAdjust', () => {

    let image;

    before(done => {
        mkdirp(tmpDir, done);
    });

    beforeEach(done => {
        lwip.open(imgs.png.trans, (err, img) => {
            image = img;
            done(err);
        });
    });

    beforeEach(() => {
        current = [basename];
    });

    afterEach(done => {
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', done);
    });

    describe('hs=0, sd=0, ld=0, ad=0', () => {
        const hs = 0,
            sd = 0,
            ld = 0,
            ad = 0;
        it('should succeed', done => {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            current.push('ad' + ad);
            image.hslaAdjust(hs, sd, ld, ad, done);
        });
    });

    describe('hs=50, sd=0.3, ld=0.4, ad=0.5', () => {
        const hs = 50,
            sd = 0.3,
            ld = 0.4,
            ad = 0.5;
        it('should succeed', done => {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            current.push('ad' + ad);
            image.hslaAdjust(hs, sd, ld, ad, done);
        });
    });

    describe('hs=-50, sd=-0.3, ld=-0.4, ad=-1', () => {
        const hs = -50,
            sd = -0.3,
            ld = -0.4,
            ad = -1;
        it('should succeed', done => {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            current.push('ad' + ad);
            image.hslaAdjust(hs, sd, ld, ad, done);
        });
    });

});
