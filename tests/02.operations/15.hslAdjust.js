var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'hslAdjust',
    current;

describe('lwip.hslAdjust', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.png.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    beforeEach(function() {
        current = [basename];
    });

    afterEach(function(done) {
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', done);
    });

    describe('hs=0, sd=0, ld=0', function() {
        var hs = 0,
            sd = 0,
            ld = 0;
        it('should succeed', function(done) {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            image.hslAdjust(hs, sd, ld, done);
        });
    });

    describe('hs=50, sd=0.3, ld=0.4', function() {
        var hs = 50,
            sd = 0.3,
            ld = 0.4;
        it('should succeed', function(done) {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            image.hslAdjust(hs, sd, ld, done);
        });
    });

    describe('hs=-50, sd=-0.3, ld=-0.4', function() {
        var hs = -50,
            sd = -0.3,
            ld = -0.4;
        it('should succeed', function(done) {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            image.hslAdjust(hs, sd, ld, done);
        });
    });

});
