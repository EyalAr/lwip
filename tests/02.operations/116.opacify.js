const join = require('path').join,
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');

describe('lwip.opacify', () => {

    let image;

    before(() => {
        return mkdirp(tmpDir);
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
