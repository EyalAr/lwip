const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'blur';
let current;

describe('lwip.blur', () => {

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

    describe('SD=1', () => {
        const sd = 1;
        it('should succeed', done => {
            current.push('sd' + sd);
            image.blur(sd, done);
        });
    });

    describe('SD=5.5', () => {
        const sd = 5.5;
        it('should succeed', done => {
            current.push('sd' + sd);
            image.blur(sd, done);
        });
    });

    describe('SD=20', () => {
        const sd = 20;
        it('should succeed', done => {
            current.push('sd' + sd);
            image.blur(sd, done);
        });
    });

});
