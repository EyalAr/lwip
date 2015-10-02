var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'saturate',
    current;

describe('lwip.saturate', function() {

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

    describe('delta=0', function() {
        var d = 0;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.saturate(d, done);
        });
    });

    describe('delta=0.8', function() {
        var d = 0.8;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.saturate(d, done);
        });
    });

    describe('delta=-1', function() {
        var d = -1;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.saturate(d, done);
        });
    });

});
