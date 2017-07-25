const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'paste';
let current;

describe('lwip.paste', () => {

    let back, front;
    // back is the regular image. front is flipped on 'y' axis.

    before(done => {
        mkdirp(tmpDir, done);
    });

    beforeEach(done => {
        lwip.open(imgs.png.rgb, (err, img) => {
            if (err) return done(err);
            back = img;
            img.clone((err, clone) => {
                if (err) return done(err);
                clone.mirror('y', (err, clone) => {
                    if (err) return done(err);
                    front = clone;
                    done();
                });
            });
        });
    });

    beforeEach(() => {
        current = [basename];
    });

    afterEach(done => {
        back.writeFile(join(tmpDir, current.join('_') + '.png'), done);
    });

    describe('background transparent, front opaque', () => {
        beforeEach(done => {
            back.fade(1, done);
        });
        it('should see flipped image', done => {
            current.push('bgTrans_frontOpaque_resultFlipped');
            back.paste(0, 0, front, done);
        });
    });

    describe('background opaque, front transparent', () => {
        beforeEach(done => {
            front.fade(1, done);
        });
        it('should see normal image', done => {
            current.push('bgOpaque_frontTrans_resultNormal');
            back.paste(0, 0, front, done);
        });
    });

    describe('background opaque, front 50%', () => {
        beforeEach(done => {
            front.fade(0.5, done);
        });
        it('should see blended image', done => {
            current.push('bgOpaque_front50_resultBlend');
            back.paste(0, 0, front, done);
        });
    });

    describe('background 50%, front opaque', () => {
        beforeEach(done => {
            back.fade(0.5, done);
        });
        it('should see flipped image', done => {
            current.push('bg50_frontOpaque_resultFlipped');
            back.paste(0, 0, front, done);
        });
    });

    describe('background 50%, front 50%', () => {
        beforeEach(done => {
            back.fade(0.5, done);
        });
        beforeEach(done => {
            front.fade(0.5, done);
        });
        it('should see blended transparent image', done => {
            current.push('bg50_front50_resultBlendTrans');
            back.paste(0, 0, front, done);
        });
    });

    describe('front scaled 50% in middle', () => {
        beforeEach(done => {
            front.scale(0.5, done);
        });
        it('should see small front in middle of back', done => {
            current.push('front_in_middle');
            back.paste(back.width() / 4, back.height() / 4, front, done);
        });
    });

});
