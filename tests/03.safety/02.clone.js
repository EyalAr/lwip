const assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('clone correct behavior', () => {

    let image;

    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            image = img;
            done(err);
        });
    });

    describe('image.clone', () => {
        it('should clone the image at the correct state', done => {
            const width = image.width(),
                height = image.height();
            image.clone((err, clone) => {
                // this callback is called asynchronously,
                // so 'resize' is called before this callback is run,
                // but still we want to make sure we get a clone of the
                // original image.
                if (err) return done(err);
                assert(clone.width() === width);
                assert(clone.height() === height);
                done();
            });
            image.resize(100, 100, err => {
                if (err) return done(err);
            });
        });
    });

});
