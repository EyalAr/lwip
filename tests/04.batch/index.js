const join = require('path').join,
    should = require('should'),
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    utils = require('../utils'),
    imgs = require('../imgs');

describe('image.batch', () => {

    let ops, batch;

    beforeEach(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            if (err) return done(err);
            batch = img.batch();
            ops = utils.generateRandomBatch(batch, 3);
            done();
        });
    });

    describe('exec', () => {
        describe('same batch twice in parallel', () => {
            it('should throw an error', () => {
                batch.exec(() => {});
                batch.exec.bind(batch, () => {}).should.throwError();
            });
        });
    });

    describe('toBuffer', () => {

        describe('jpeg', () => {

            describe('quality 0', () => {
                it('should succeed', done => {
                    batch.toBuffer('jpg', {
                        quality: 0
                    }, (err, buffer) => {
                        done(err);
                    });
                });
            });

            describe('quality 100', () => {
                it('should succeed', done => {
                    batch.toBuffer('jpg', {
                        quality: 100
                    }, (err, buffer) => {
                        done(err);
                    });
                });
            });

        });

        describe('png', () => {

            describe('non interlaced', () => {

                describe('no compression', () => {
                    it('should succeed', done => {
                        batch.toBuffer('png', {
                            interlaced: false,
                            compression: 'none',
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

                describe('fast compression', () => {
                    it('should succeed', done => {
                        batch.toBuffer('png', {
                            interlaced: false,
                            compression: 'fast',
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

                describe('high compression', () => {
                    it('should succeed', done => {
                        batch.toBuffer('png', {
                            interlaced: false,
                            compression: 'high',
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

            });

            describe('interlaced', () => {

                describe('no compression', () => {
                    it('should succeed', done => {
                        batch.toBuffer('png', {
                            interlaced: true,
                            compression: 'none',
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

                describe('fast compression', () => {
                    it('should succeed', done => {
                        batch.toBuffer('png', {
                            interlaced: true,
                            compression: 'fast',
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

                describe('high compression', () => {
                    it('should succeed', done => {
                        batch.toBuffer('png', {
                            interlaced: true,
                            compression: 'high',
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

            });

        });

        describe('gif', () => {

            describe('non interlaced', () => {

                describe('no transparency', () => {
                    it('should succeed', done => {
                        batch.toBuffer('gif', {
                            colors: 122,
                            interlaced: false,
                            transparency: false
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

                describe('with transparency', () => {
                    it('should succeed', done => {
                        batch.toBuffer('gif', {
                            interlaced: false,
                            transparency: true,
                            threshold: 55
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

            });

            describe('interlaced', () => {

                describe('no transparency', () => {
                    it('should succeed', done => {
                        batch.toBuffer('gif', {
                            colors: 122,
                            interlaced: true,
                            transparency: false
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

                describe('with transparency', () => {
                    it('should succeed', done => {
                        batch.toBuffer('gif', {
                            interlaced: true,
                            transparency: true,
                            threshold: 55
                        }, (err, buffer) => {
                            done(err);
                        });
                    });
                });

            });

        });

    });

    describe('writeFile', () => {

        const tmpDir = join(__dirname, '../results');

        before(done => {
            mkdirp(tmpDir, done);
        });

        describe('jpeg', () => {

            describe('quality 0', () => {
                it('should succeed', done => {
                    batch.writeFile(join(tmpDir, 'btch-q0-' + ops.join('#') + '.jpg'), 'jpg', {
                        quality: 0
                    }, done);
                });
            });

            describe('quality 100', () => {
                it('should succeed', done => {
                    batch.writeFile(join(tmpDir, 'btch-q100-' + ops.join('#') + '.jpg'), 'jpg', {
                        quality: 100
                    }, done);
                });
            });

        });

        describe('png', () => {

            describe('non interlaced', () => {

                describe('no compression', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch--noint#nocomp--' + ops.join('#') + '.png'), 'png', {
                            interlaced: false,
                            compression: 'none',
                        }, done);
                    });
                });

                describe('fast compression', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch--noint#fstcomp--' + ops.join('#') + '.png'), 'png', {
                            interlaced: false,
                            compression: 'fast',
                        }, done);
                    });
                });

                describe('high compression', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch-noint#hicomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: false,
                            compression: 'high',
                        }, done);
                    });
                });

            });

            describe('interlaced', () => {

                describe('no compression', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch-intr#nocomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: true,
                            compression: 'none',
                        }, done);
                    });
                });

                describe('fast compression', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch-intr#fstcomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: true,
                            compression: 'fast',
                        }, done);
                    });
                });

                describe('high compression', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch-intr#hicomp-' + ops.join('#') + '.png'), 'png', {
                            interlaced: true,
                            compression: 'high',
                        }, done);
                    });
                });

            });

        });

        describe('gif', () => {

            describe('non interlaced', () => {

                describe('no transparency', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch--noint#notrn--' + ops.join('#') + '.gif'), 'gif', {
                            interlaced: false,
                            transparency: false,
                        }, done);
                    });
                });

                describe('with transparency', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch--noint#trn--' + ops.join('#') + '.gif'), 'gif', {
                            interlaced: false,
                            transparency: true,
                        }, done);
                    });
                });

            });

            describe('interlaced', () => {

                describe('no transparency', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch--noint#notrn--' + ops.join('#') + '.gif'), 'gif', {
                            interlaced: true,
                            transparency: false,
                        }, done);
                    });
                });

                describe('with transparency', () => {
                    it('should succeed', done => {
                        batch.writeFile(join(tmpDir, 'btch--noint#trn--' + ops.join('#') + '.gif'), 'gif', {
                            interlaced: true,
                            transparency: true,
                        }, done);
                    });
                });

            });

        });

    });

});
