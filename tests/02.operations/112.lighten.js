const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'lighten';
let current;

describe('lwip.lighten', () => {

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
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', {
            compression: 'fast',
            interlaced: true
        }, done);
    });

    describe('delta=0', () => {
        const d = 0;
        it('should succeed', done => {
            current.push('d' + d);
            image.lighten(d, done);
        });
    });

    describe('delta=0.5', () => {
        const d = 0.5;
        it('should succeed', done => {
            current.push('d' + d);
            image.lighten(d, done);
        });
    });

    describe('delta=-0.5', () => {
        const d = -0.5;
        it('should succeed', done => {
            current.push('d' + d);
            image.lighten(d, done);
        });
    });

});
