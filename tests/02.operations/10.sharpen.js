var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'sharpen',
    current;

describe('lwip.sharpen', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    beforeEach(function() {
        current = [basename];
    });

    afterEach(function(done) {
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('amp=0', function() {
        var amp = 0;
        it('should succeed', function(done) {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=5.5', function() {
        var amp = 5.5;
        it('should succeed', function(done) {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=99', function() {
        var amp = 99;
        it('should succeed', function(done) {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=-5.5', function() {
        var amp = -5.5;
        it('should succeed', function(done) {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

    describe('amp=-99', function() {
        var amp = -99;
        it('should succeed', function(done) {
            current.push('amp' + amp);
            image.sharpen(amp, done);
        });
    });

});
