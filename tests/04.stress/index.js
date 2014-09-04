var join = require('path').join,
    assert = require('assert'),
    async = require('async'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
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

    describe('open image 500 times (in parallel) and save to disk as png (high compression, interlaced)', function() {
        it('should succeed', function(done) {
            async.times(500, function(i, done) {
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

    describe('10 random manipulations for 100 images', function() {
        it('should succeed', function(done) {
            async.times(100, function(i, done) {
                lwip.open(imgs.jpg.rgb, 'jpeg', function(err, image) {
                    if (err) return done(err);
                    var batch = image.batch();
                    for (var i = 0; i < 10; i++) {
                        var r = Math.floor(Math.random() * 5);
                        switch (r) {
                            case 0:
                                batch = batch.blur(5);
                                break;
                            case 1:
                                batch = batch.rotate(45);
                                break;
                            case 2:
                                batch = batch.resize(600, 200);
                                break;
                            case 3:
                                batch = batch.crop(100, 150);
                                break;
                            case 0:
                                batch = batch.scale(1.1, 0.66);
                                break;
                        }
                    }
                    batch.writeFile(outpathJpeg, 'jpeg', {
                        quality: 50
                    }, done);
                });
            }, done);
        });
    });

    describe('rotate an image 100 times (up to 90degs)', function() {
        it('should succeed', function(done) {
            var a = 0.9;
            lwip.open(imgs.jpg.rgb, 'jpeg', function(err, image) {
                if (err) return done(err);
                async.timesSeries(100, function(i, done) {
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
