// methods should throw errors when arguments are invalid

var should = require("should"),
    assert = require('assert'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.paste arguments validation', function() {

    var batch;
    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch();
            done(err);
        });
    });

    describe('invalid pasted image (1)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, {}, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (2)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, null, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (3)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (4)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, function() {}, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (5)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, undefined, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (6)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, 0, function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (7)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, "foo", function() {}).should.throwError();
        });
    });

    describe('invalid pasted image (8)', function() {
        it('should throw an error', function() {
            batch.paste.bind(batch, 0, 0, [1, 2, 3], function() {}).should.throwError();
        });
    });

    describe('pasted image exceeds dimensions', function() {

        var Image = require('../../lib/Image');

        var pst;
        beforeEach(function(done) {
            lwip.open(imgs.png.rgb, function(err, img) {
                pst = img;
                done(err);
            });
        });

        describe('at the time of exec', function() {
            it('should return an error', function(done) {
                assert(pst instanceof Image);
                batch.paste.bind(batch, 10, 10, pst).should.not.throwError();
                batch.exec(function(err) {
                    // there should be an error message
                    assert(!!err);
                    done();
                });
            });
        });

        describe('before exec', function() {
            it('should not return an error', function(done) {
                assert(pst instanceof Image);
                batch.paste.bind(batch, 10, 10, pst).should.not.throwError();
                pst.scale(0.1, function(err, pst) {
                    batch.exec(function(err) {
                        // there should not be an error message
                        assert(!err);
                        done();
                    });
                });
            });
        });
    });
});
