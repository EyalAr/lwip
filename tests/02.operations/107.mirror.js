const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'mirror';
let current;

describe('lwip.mirror (/flip)', () => {

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

    afterEach(done => {
        image.writeFile(join(tmpDir, current.join('_') + '.gif'), 'gif', {
            colors: 256
        }, done);
    });

    describe('around one axis', () => {

        beforeEach(() => {
            current = [basename, 'axis'];
        });

        describe('x', () => {
            it('should succeed', done => {
                current.push('x');
                image.mirror('x', done);
            });
        });

        describe('y', () => {
            it('should succeed', done => {
                current.push('y');
                image.flip('y', done);
            });
        });

    });

    describe('around two axes', () => {

        beforeEach(() => {
            current = [basename, 'axes'];
        });

        describe('xy', () => {
            it('should succeed', done => {
                current.push('xy');
                image.flip('xy', done);
            });
        });

        describe('yx', () => {
            it('should succeed', done => {
                current.push('yx');
                image.mirror('yx', done);
            });
        });

    });

});
