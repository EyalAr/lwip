var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    utils = require('../utils'),
    imgs = require('../imgs');

describe('image.batch', function() {

    var ops, batch;

    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            if (err) return done(err);
            batch = img.batch();
            ops = utils.generateRandomBatch(batch, 3);
            done();
        });
    });

    describe('exec', function() {
        describe('same batch twice in parallel', function() {
            it('should throw an error', function() {
                batch.exec(function() {});
                batch.exec.bind(batch, function() {}).should.throwError();
            });
        });
    });

    describe('toBuffer', function() {

        describe('jpeg', function() {

            describe('quality 0', function() {
                it('should succeed', function(done) {
                    batch.toBuffer('jpg', {
                        quality: 0
                    }, function(err, buffer) {
                        done(err);
                    });
                });
            });

            describe('quality 100', function() {
                it('should succeed', function(done) {
                    batch.toBuffer('jpg', {
                        quality: 100
                    }, function(err, buffer) {
                        done(err);
                    });
                });
            });

        });

        describe('png', function() {

            describe('non interlaced', function() {

                describe('no compression', function() {
                    it('should succeed', function(done) {
                        batch.toBuffer('png', {
                            interlaced: false,
                            compression: 'none',
                        }, function(err, buffer) {
                            done(err);
                        });
                    });
                });

                describe('fast compression', function() {
                    it('should succeed', function(done) {
                        batch.toBuffer('png', {
                            interlaced: false,
                            compression: 'fast',
                        }, function(err, buffer) {
                            done(err);
                        });
                    });
                });

                describe('high compression', function() {
                    it('should succeed', function(done) {
                        batch.toBuffer('png', {
                            interlaced: false,
                            compression: 'high',
                        }, function(err, buffer) {
                            done(err);
                        });
                    });
                });

            });

            describe('interlaced', function() {

                describe('no compression', function() {
                    it('should succeed', function(done) {
                        batch.toBuffer('png', {
                            interlaced: true,
                            compression: 'none',
                        }, function(err, buffer) {
                            done(err);
                        });
                    });
                });

                describe('fast compression', function() {
                    it('should succeed', function(done) {
                        batch.toBuffer('png', {
                            interlaced: true,
                            compression: 'fast',
                        }, function(err, buffer) {
                            done(err);
                        });
                    });
                });

                describe('high compression', function() {
                    it('should succeed', function(done) {
                        batch.toBuffer('png', {
                            interlaced: true,
                            compression: 'high',
                        }, function(err, buffer) {
                            done(err);
                        });
                    });
                });

            });

        });

    });

    describe('writeFile', function() {

        var tmpDir = join(__dirname, '../results');

        before(function(done) {
            mkdirp(tmpDir, done);
        });

        describe('jpeg', function() {

            describe('quality 0', function() {
                it('should succeed', function(done) {
                    batch.writeFile(join(tmpDir, 'btch-q0-' + ops.join('#') + '.jpg'), 'jpg', {
                        quality: 0
                    }, done);
                });
            });

            describe('quality 100', function() {
                it('should succeed', function(done) {
                    batch.writeFile(join(tmpDir, 'btch-q100-' + ops.join('#') + '.jpg'), 'jpg', {
                        quality: 100
                    }, done);
                });
            });

        });

        describe('png', function() {

            describe('non interlaced', function() {

                describe('no compression', function() {
                    it('should succeed', function(done) {
                        batch.writeFile(join(tmpDir, 'btch--noint#nocomp--' + ops.join('#') + '.png'), 'png', {
                            interlaced: false,
                            compression: 'none',
                        }, done);
                    });
                });

                describe('fast compression', function() {
                    it('should succeed', function(done) {
                        batch.writeFile(join(tmpDir, 'btch--noint#fstcomp--' + ops.join('#') + '.png'), 'png', {
                            interlaced: false,
                            compression: 'fast',
                        }, done);
                    });
                });

                describe('high compression', function() {
                    it('should succeed', function(done) {
                        batch.writeFile(join(tmpDir, 'btch-noint#hicomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: false,
                            compression: 'high',
                        }, done);
                    });
                });

            });

            describe('interlaced', function() {

                describe('no compression', function() {
                    it('should succeed', function(done) {
                        batch.writeFile(join(tmpDir, 'btch-intr#nocomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: true,
                            compression: 'none',
                        }, done);
                    });
                });

                describe('fast compression', function() {
                    it('should succeed', function(done) {
                        batch.writeFile(join(tmpDir, 'btch-intr#fstcomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: true,
                            compression: 'fast',
                        }, done);
                    });
                });

                describe('high compression', function() {
                    it('should succeed', function(done) {
                        batch.writeFile(join(tmpDir, 'btch-intr#hicomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: true,
                            compression: 'high',
                        }, done);
                    });
                });

            });

        });

    });

});
