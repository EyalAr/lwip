var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results');

describe('lwip.setMetadata', function() {

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    describe('set png metadata', function() {
        it('should set metadata and be able to read metadata', function(done) {
            var filename = 'setMetadata.png';
            var metadata = 'The quick brown fox jumps over the lazy dog';

            lwip.create(1, 1, function(err, img) {
                if (err) return done(err);
                img.setMetadata(metadata);
                img.writeFile(join(tmpDir, filename), function(err) {
                    lwip.open(join(tmpDir, filename), function(err, imgWithMetadata) {
                        if (err) return done(err);
                        assert(imgWithMetadata.getMetadata() === metadata);
                        done();
                    })
                });
            });
        });

        it('should not set metadata if setMetadata is not called', function(done) {
            var filename = 'noMetadata.png';

            lwip.create(1, 1, function(err, img) {
                if (err) return done(err);

                img.writeFile(join(tmpDir, filename), function(err) {
                    lwip.open(join(tmpDir, filename), function(err, imgNoMetadata) {
                        if (err) return done(err);
                        assert(imgNoMetadata.getMetadata() === null);
                        done();
                    })
                });
            });
        });
    });

});
