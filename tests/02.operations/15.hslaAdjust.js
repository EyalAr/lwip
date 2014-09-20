var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'hslaAdjust',
    current;

describe('lwip.hslaAdjust', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.png.trans, function(err, img) {
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

    describe('hs=0, sd=0, ld=0, td=0', function() {
        var hs = 0,
            sd = 0,
            ld = 0,
            td = 0;
        it('should succeed', function(done) {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            current.push('td' + td);
            image.hslaAdjust(hs, sd, ld, td, done);
        });
    });

    describe('hs=50, sd=0.3, ld=0.4, td=0.5', function() {
        var hs = 50,
            sd = 0.3,
            ld = 0.4,
            td = 0.5;
        it('should succeed', function(done) {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            current.push('td' + td);
            image.hslaAdjust(hs, sd, ld, td, done);
        });
    });

    describe('hs=-50, sd=-0.3, ld=-0.4, td=-1', function() {
        var hs = -50,
            sd = -0.3,
            ld = -0.4,
            td = -1;
        it('should succeed', function(done) {
            current.push('hs' + hs);
            current.push('sd' + sd);
            current.push('ld' + ld);
            current.push('td' + td);
            image.hslaAdjust(hs, sd, ld, td, done);
        });
    });

});
