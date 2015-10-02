var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'darken',
    current;

describe('lwip.darken', function() {

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
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('delta=0', function() {
        var d = 0;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.darken(d, done);
        });
    });

    describe('delta=0.5', function() {
        var d = 0.5;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.darken(d, done);
        });
    });

    describe('delta=-0.5', function() {
        var d = -0.5;
        it('should succeed', function(done) {
            current.push('d' + d);
            image.darken(d, done);
        });
    });

});
