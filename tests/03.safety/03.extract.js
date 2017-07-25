const join = require('path').join,
    lwip = require('../../'),
    imgs = require('../imgs');

describe('extract correct behavior', () => {

    const tmpDir = join(__dirname, '../results');
    let image;

    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('image.extract', () => {
        it('should extract the sub-image at the correct state', done => {
            image.extract(50, 50, 339, 299, (err, extimg) => {
                // this callback is called asynchronously,
                // so 'hue' is called before this callback is run,
                // but still we want to make sure the extracted image has the
                // original hue.
                if (err) return done(err);
                extimg.writeFile(join(tmpDir, 'extract_behaviour_SHOULD_HAVE_ORIGINAL_HUE.jpg'), done);
            });
            image.hue(100, err => {
                if (err) return done(err);
            });
        });
    });

});
