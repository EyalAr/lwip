const join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs'),
    should = require('should');

const tmpDir = join(__dirname, '../results');

describe('lwip.setMetadata', () => {

    before(done => {
        mkdirp(tmpDir, done);
    });

    describe('set png metadata', () => {
        it('should set metadata and be able to read metadata', done => {
            const filename = 'setMetadata.png';
            const metadata = 'The quick brown fox jumps over the lazy dog';

            lwip.create(1, 1, (err, img) => {
                if (err) return done(err);
                img.setMetadata(metadata);
                img.writeFile(join(tmpDir, filename), err => {
                    lwip.open(join(tmpDir, filename), (err, imgWithMetadata) => {
                        if (err) return done(err);
                        assert(imgWithMetadata.getMetadata() === metadata);
                        done();
                    });
                });
            });
        });

        it('should not set metadata if setMetadata is not called', done => {
            const filename = 'noMetadata.png';

            lwip.create(1, 1, (err, img) => {
                if (err) return done(err);

                img.writeFile(join(tmpDir, filename), err => {
                    lwip.open(join(tmpDir, filename), (err, imgNoMetadata) => {
                        if (err) return done(err);
                        assert(imgNoMetadata.getMetadata() === null);
                        done();
                    });
                });
            });
        });

        it('should throw error if non-string metadata is set', done => {
            const filename = 'noMetadata.png';

            lwip.create(1, 1, (err, img) => {
                if (err) return done(err);;
                img.setMetadata.bind(img, {}).should.throwError();
                img.setMetadata.bind(img, () => {}).should.throwError();
                img.setMetadata.bind(img, 42).should.throwError();
                done();
            });
        });

        it('should remove metadata if called with null parameter', done => {
            lwip.open(imgs.png.hasMetadata, (err, img) => {
                const filename = 'noMetadata.png';

                assert(img.getMetadata() === 'Lorem ipsum dolor sit amet');

                img.setMetadata(null);

                img.writeFile(join(tmpDir, filename), err => {
                    lwip.open(join(tmpDir, filename), (err, imgWithMetadata) => {
                        if (err) return done(err);
                        assert(imgWithMetadata.getMetadata() === null);
                        done();
                    });
                });
            });
        });

        it('can reset metadata on image with existing metadata', done => {
            lwip.open(imgs.png.hasMetadata, (err, img) => {
                const filename = 'changedMetadata.png';
                const oldMetadata = 'Lorem ipsum dolor sit amet';
                const newMetadata = 'The quick brown fox jumps over the lazy dog';

                assert(img.getMetadata() === oldMetadata);
                img.setMetadata(newMetadata);

                img.writeFile(join(tmpDir, filename), err => {
                    lwip.open(join(tmpDir, filename), (err, imgWithMetadata) => {
                        if (err) return done(err);
                        assert(imgWithMetadata.getMetadata() === newMetadata);
                        done();
                    });
                });
            });
        });
    });

});
