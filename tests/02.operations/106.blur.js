var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'blur',
    current;

describe('lwip.blur', function() {

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

    describe('SD=1', function() {
        var sd = 1;
        it('should succeed', function(done) {
            current.push('sd' + sd);
            image.blur(sd, done);
        });
    });

    describe('SD=5.5', function() {
        var sd = 5.5;
        it('should succeed', function(done) {
            current.push('sd' + sd);
            image.blur(sd, done);
        });
    });

    describe('SD=20', function() {
        var sd = 20;
        it('should succeed', function(done) {
            current.push('sd' + sd);
            image.blur(sd, done);
        });
    });

});
