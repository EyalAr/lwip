var join = require('path').join,
    assert = require('assert'),
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'batch',
    current;

describe('image.batch', function() {

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    var batch;
    beforeEach(function(done) {
        lwip.open(imgs.jpg.rgb, function(err, img) {
            batch = img.batch();
            done(err);
        });
    });

    beforeEach(function() {
        current = [basename];
    });

    afterEach(function(done) {
        batch.writeFile(join(tmpDir, current.join('_') + '.jpg'), 'jpeg', {
            quality: 100
        }, done);
    });

    describe('rotate(45,yellow)->crop(200,200)->blur(5)', function() {
        it('should succeed', function() {
            current.push('rotate45yellow', 'crop200X200', 'blur5');
            batch.rotate(45, 'yellow').crop(200, 200).blur(5);
        });
    });

    describe('rotate(-20,green)->scale(2)->crop(500,500)', function() {
        it('should succeed', function() {
            current.push('rotate-20green', 'scale2', 'crop500X500');
            batch.rotate(-20, 'green').scale(2).crop(500, 500);
        });
    });

});
