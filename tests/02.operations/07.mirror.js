var join = require('path').join,
    should = require('should'),
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
        image.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
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

        describe('Y', function() {
            it('should succeed', function(done) {
                current.push('Y');
                image.flip('Y', done);
            });
        });

    });

    describe('around two axes', function() {

        beforeEach(function() {
            current = [basename, 'axes'];
        });

        describe('Xy', function() {
            it('should succeed', function(done) {
                current.push('Xy');
                image.flip('Xy', done);
            });
        });

        describe('YX', function() {
            it('should succeed', function(done) {
                current.push('YX');
                image.mirror('YX', done);
            });
        });

    });

});
