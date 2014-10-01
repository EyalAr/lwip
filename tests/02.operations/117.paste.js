var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'paste',
    current;

describe('lwip.paste', function() {

    var back, front;
    // back is the regular image. front is flipped on 'y' axis.

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.png.rgb, function(err, img) {
            if (err) return done(err);
            back = img;
            img.clone(function(err, clone) {
                if (err) return done(err);
                clone.mirror('y', function(err, clone) {
                    if (err) return done(err);
                    front = clone;
                    done();
                });
            });
        });
    });

    beforeEach(function() {
        current = [basename];
    });

    afterEach(function(done) {
        back.writeFile(join(tmpDir, current.join('_') + '.png'), done);
    });

    describe('background transparent, front opaque', function() {
        beforeEach(function(done) {
            back.fade(1, done);
        });
        it('should see flipped image', function(done) {
            current.push('bgTrans_frontOpaque_resultFlipped');
            back.paste(0, 0, front, done);
        });
    });

    describe('background opaque, front transparent', function() {
        beforeEach(function(done) {
            front.fade(1, done);
        });
        it('should see normal image', function(done) {
            current.push('bgOpaque_frontTrans_resultNormal');
            back.paste(0, 0, front, done);
        });
    });

    describe('background opaque, front 50%', function() {
        beforeEach(function(done) {
            front.fade(0.5, done);
        });
        it('should see blended image', function(done) {
            current.push('bgOpaque_front50_resultBlend');
            back.paste(0, 0, front, done);
        });
    });

    describe('background 50%, front opaque', function() {
        beforeEach(function(done) {
            back.fade(0.5, done);
        });
        it('should see flipped image', function(done) {
            current.push('bg50_frontOpaque_resultFlipped');
            back.paste(0, 0, front, done);
        });
    });

    describe('background 50%, front 50%', function() {
        beforeEach(function(done) {
            back.fade(0.5, done);
        });
        beforeEach(function(done) {
            front.fade(0.5, done);
        });
        it('should see blended transparent image', function(done) {
            current.push('bg50_front50_resultBlendTrans');
            back.paste(0, 0, front, done);
        });
    });

    describe('front scaled 50% in middle', function() {
        beforeEach(function(done) {
            front.scale(0.5, done);
        });
        it('should see small front in middle of back', function(done) {
            current.push('front_in_middle');
            back.paste(back.width() / 4, back.height() / 4, front, done);
        });
    });

});
