var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'mirror',
    current;

describe('lwip.mirror (/flip)', function() {

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

    afterEach(function(done) {
        image.writeFile(join(tmpDir, current.join('_') + '.gif'), 'gif', {
            colors: 256
        }, done);
    });

    describe('around one axis', function() {

        beforeEach(function() {
            current = [basename, 'axis'];
        });

        describe('x', function() {
            it('should succeed', function(done) {
                current.push('x');
                image.mirror('x', done);
            });
        });

        describe('y', function() {
            it('should succeed', function(done) {
                current.push('y');
                image.flip('y', done);
            });
        });

    });

    describe('around two axes', function() {

        beforeEach(function() {
            current = [basename, 'axes'];
        });

        describe('xy', function() {
            it('should succeed', function(done) {
                current.push('xy');
                image.flip('xy', done);
            });
        });

        describe('yx', function() {
            it('should succeed', function(done) {
                current.push('yx');
                image.mirror('yx', done);
            });
        });

    });

});
