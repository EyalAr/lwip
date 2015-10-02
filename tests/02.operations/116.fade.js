var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'fade',
    current;

describe('lwip.fade', function() {

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
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', {
            compression: 'fast',
            interlaced: true
        }, done);
    });

    describe('delta=0', function() {
        var d = 0;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.fade(d, done);
        });
    });

    describe('delta=0.5', function() {
        var d = 0.5;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.fade(d, done);
        });
    });

    describe('delta=-0.5', function() {
        var d = -0.5;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.fade(d, done);
        });
    });

    describe('delta=1', function() {
        var d = 1;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.fade(d, done);
        });
    });

    describe('delta=-1', function() {
        var d = -1;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.fade(d, done);
        });
    });

});
