const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');

describe('lwip.opacify', () => {

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

    afterEach(done => {
        image.writeFile(join(tmpDir, 'opacify.png'), 'png', {
            compression: 'fast',
            interlaced: true
        }, done);
    });

    it('should succeed', done => {
        image.opacify(done);
    });

});
