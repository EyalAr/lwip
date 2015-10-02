var join = require('path').join,
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

var tmpDir = join(__dirname, '../results'),
    basename = 'rotate',
    current;

describe('lwip.rotate', function() {

    var image;

    before(function(done) {
        mkdirp(tmpDir, done);
    });

    beforeEach(function(done) {
        lwip.open(imgs.gif.trans, function(err, img) {
            image = img;
            done(err);
        });
    });

    afterEach(function(done) {
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', {
            compressions: 'fast'
        }, done);
    });

    describe('right angles', function() {

        beforeEach(function() {
            current = [basename];
        });

        describe('90 degs', function() {
            it('should succeed', function(done) {
                current.push(90, 'degs');
                image.rotate(90, done);
            });
        });

        describe('180 degs', function() {
            it('should succeed', function(done) {
                current.push(180, 'degs');
                image.rotate(180, done);
            });
        });

        describe('270 degs', function() {
            it('should succeed', function(done) {
                current.push(270, 'degs');
                image.rotate(270, done);
            });
        });

        describe('360 degs', function() {
            it('should succeed', function(done) {
                current.push(360, 'degs');
                image.rotate(360, done);
            });
        });

        describe('450 degs', function() {
            it('should succeed', function(done) {
                current.push(450, 'degs');
                image.rotate(450, done);
            });
        });

        describe('-90 degs', function() {
            it('should succeed', function(done) {
                current.push(-90, 'degs');
                image.rotate(-90, done);
            });
        });

        describe('-180 degs', function() {
            it('should succeed', function(done) {
                current.push(-180, 'degs');
                image.rotate(-180, done);
            });
        });

    });

    describe('arbitrary angles and fills', function() {

        beforeEach(function() {
            current = [basename];
        });

        describe('33 degs, unspecified fill', function() {
            it('should succeed', function(done) {
                current.push(33, 'degs', 'unspecifiedFill');
                image.rotate(33, done);
            });
        });

        describe('-33 degs, "yellow" fill', function() {
            it('should succeed', function(done) {
                current.push(-33, 'degs', 'yellow');
                image.rotate(-33, 'yellow', done);
            });
        });

        describe('45 degs, [50,100,300,75] fill', function() {
            it('should succeed', function(done) {
                current.push(45, 'degs', [50, 100, 250,75]);
                image.rotate(45, [50, 100, 250, 75], done);
            });
        });

        describe('-5 degs, {r:200,g:110,b:220} fill', function() {
            it('should succeed', function(done) {
                current.push(-5, 'degs', 'r-200,g-110,b-220');
                image.rotate(-5, {
                    r: 200,
                    g: 110,
                    b: 220
                }, done);
            });
        });

        describe('-5 degs, {r:200,g:110,b:220,a:50} fill', function() {
            it('should succeed', function(done) {
                current.push(-5, 'degs', 'r-200,g-110,b-220,a-50');
                image.rotate(-5, {
                    r: 200,
                    g: 110,
                    b: 220,
                    a: 50
                }, done);
            });
        });

    });

});
