const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'saturate';
let current;

describe('lwip.saturate', () => {

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

    describe('delta=0', () => {
        const d = 0;
        it('should succeed', done => {
            current.push('d' + d);
            image.saturate(d, done);
        });
    });

    describe('delta=0.8', () => {
        const d = 0.8;
        it('should succeed', done => {
            current.push('d' + d);
            image.saturate(d, done);
        });
    });

    describe('delta=-1', () => {
        const d = -1;
        it('should succeed', done => {
            current.push('d' + d);
            image.saturate(d, done);
        });
    });

});
