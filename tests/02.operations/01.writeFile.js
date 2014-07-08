var join = require('path').join,
    should = require("should"),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    outJpeg = 'write_test.jpg',
    outpath = join(tmpDir, outJpeg);

describe('lwip.writeFile', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });
    before(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            image = img;
            done(err);
        });
    });

    describe('jpeg', function() {

        describe('with type unspecified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpath, done);
                });
            });

            describe('params specified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpath, {
                        quality: 20
                    }, done);
                });
            });

        });

        describe('with type specified', function() {

            describe('params unspecified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpath, 'jpeg', done);
                });
            });

            describe('params specified', function() {
                it('should succeed', function(done) {
                    image.writeFile(outpath, 'jpeg', {
                        quality: 20
                    }, done);
                });
            });

        });

    });
});
