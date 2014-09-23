var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../');

var tmpDir = join(__dirname, '../results');

describe('lwip.create', function() {

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    describe('500X500, [255,0,0,50]', function() {
        it('should succeed', function(done) {
            var width = 500,
                height = 500,
                bg = [255, 0, 0, 50];
            lwip.create(width, height, bg, function(err, img) {
                if (err) return done(err);
                assert(img.width() === width);
                assert(img.height() === height);
                img.writeFile(join(tmpDir, 'create500X500[255,0,0,50].png'), done);
            });
        });
    });

    describe('500X500, [0,255,0]', function() {
        it('should succeed', function(done) {
            var width = 500,
                height = 500,
                bg = [0, 255, 0];
            lwip.create(width, height, bg, function(err, img) {
                if (err) return done(err);
                assert(img.width() === width);
                assert(img.height() === height);
                img.writeFile(join(tmpDir, 'create500X500[0,255,0].png'), done);
            });
        });
    });

    describe('500X500, no color specified', function() {
        it('should succeed', function(done) {
            var width = 500,
                height = 500;
            lwip.create(width, height, function(err, img) {
                if (err) return done(err);
                assert(img.width() === width);
                assert(img.height() === height);
                img.writeFile(join(tmpDir, 'create500X500nocolor.png'), done);
            });
        });
    });

});
