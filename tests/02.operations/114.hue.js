var join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'hue',
    current;

describe('lwip.hue', function() {

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

    describe('shift=0', function() {
        var d = 0;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=50.5', function() {
        var d = 50.5;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=-50', function() {
        var d = -50;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=500', function() {
        var d = 500;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

    describe('shift=-500', function() {
        var d = -500;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.hue(d, done);
        });
    });

});
