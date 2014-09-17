var join = require('path').join,
    fs = require('fs'),
    assert = require('assert'),
    async = require('async'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    utils = require('../utils'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    outpathJpeg = join(tmpDir, 'stress.jpg'),
    outpathPng = join(tmpDir, 'stress.png');

describe('stress tests', function() {

    this.timeout(120000); // 120 seconds per test

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    describe('open image 500 times (in parallel) and save to disk as jpeg', function() {
        it('should succeed', function(done) {
            async.times(500, function(i, done) {
                lwip.open(imgs.png.rgb, 'png', function(err, image) {
                    if (err) return done(err);
                    image.writeFile(outpathJpeg, 'jpeg', {
                        quality: 50
                    }, done);
                });
            }, done);
        });
    });

    describe('open image 100 times (in parallel) and save to disk as png (high compression, interlaced)', function() {
        it('should succeed', function(done) {
            async.times(100, function(i, done) {
                lwip.open(imgs.jpg.rgb, 'jpeg', function(err, image) {
                    if (err) return done(err);
                    image.writeFile(outpathPng, 'png', {
                        compression: 'high',
                        interlaced: true
                    }, done);
                });
            }, done);
        });
    });

    describe('open image 500 times (in parallel) and save to disk as png (fast compression, not interlaced)', function() {
        it('should succeed', function(done) {
            async.times(500, function(i, done) {
                lwip.open(imgs.jpg.rgb, 'jpeg', function(err, image) {
                    if (err) return done(err);
                    image.writeFile(outpathPng, 'png', {
                        compression: 'fast',
                        interlaced: false
                    }, done);
                });
            }, done);
        });
    });

    describe('10 random manipulations for 50 images (in parallel)', function() {
        it('should succeed', function(done) {
            async.times(50, function(i, done) {
                lwip.open(imgs.jpg.rgb, 'jpeg', function(err, image) {
                    if (err) return done(err);
                    var batch = image.batch();
                    var ops = utils.generateRandomBatch(batch, 10);
                    batch.writeFile(join(tmpDir, 'stress-rnd-' + i + '.jpg'), 'jpeg', {
                        quality: 50
                    }, function(err) {
                        if (err) return done(err);
                        var data = ops.join('\n');
                        fs.writeFile(join(tmpDir, 'stress-rnd-' + i + '.txt'), data, done);
                    });
                });
            }, done);
        });
    });

    describe('rotate an image 50 times (up to 90degs)', function() {
        it('should succeed', function(done) {
            var a = 1.8;
            lwip.open(imgs.jpg.rgb, 'jpeg', function(err, image) {
                if (err) return done(err);
                async.timesSeries(50, function(i, done) {
                    image.rotate(a, 'black', done);
                }, function(err) {
                    if (err) return done(err);
                    image.writeFile(outpathJpeg, 'jpeg', {
                        quality: 90
                    }, done);
                });
            });
        });
    });

});
