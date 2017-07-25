const join = require('path').join,
    mkdirp = require('mkdirp'),
    lwip = require('../../'),
    imgs = require('../imgs');

const tmpDir = join(__dirname, '../results');
    basename = 'rotate';
let current;

describe('lwip.rotate', () => {

    let image;

    before(done => {
        mkdirp(tmpDir, done);
    });

    beforeEach(done => {
        lwip.open(imgs.gif.trans, (err, img) => {
            image = img;
            done(err);
        });
    });

    afterEach(done => {
        image.writeFile(join(tmpDir, current.join('_') + '.png'), 'png', {
            compressions: 'fast'
        }, done);
    });

    describe('right angles', () => {

        beforeEach(() => {
            current = [basename];
        });

        describe('90 degs', () => {
            it('should succeed', done => {
                current.push(90, 'degs');
                image.rotate(90, done);
            });
        });

        describe('180 degs', () => {
            it('should succeed', done => {
                current.push(180, 'degs');
                image.rotate(180, done);
            });
        });

        describe('270 degs', () => {
            it('should succeed', done => {
                current.push(270, 'degs');
                image.rotate(270, done);
            });
        });

        describe('360 degs', () => {
            it('should succeed', done => {
                current.push(360, 'degs');
                image.rotate(360, done);
            });
        });

        describe('450 degs', () => {
            it('should succeed', done => {
                current.push(450, 'degs');
                image.rotate(450, done);
            });
        });

        describe('-90 degs', () => {
            it('should succeed', done => {
                current.push(-90, 'degs');
                image.rotate(-90, done);
            });
        });

        describe('-180 degs', () => {
            it('should succeed', done => {
                current.push(-180, 'degs');
                image.rotate(-180, done);
            });
        });

    });

    describe('arbitrary angles and fills', () => {

        beforeEach(() => {
            current = [basename];
        });

        describe('33 degs, unspecified fill', () => {
            it('should succeed', done => {
                current.push(33, 'degs', 'unspecifiedFill');
                image.rotate(33, done);
            });
        });

        describe('-33 degs, "yellow" fill', () => {
            it('should succeed', done => {
                current.push(-33, 'degs', 'yellow');
                image.rotate(-33, 'yellow', done);
            });
        });

        describe('45 degs, [50,100,300,75] fill', () => {
            it('should succeed', done => {
                current.push(45, 'degs', [50, 100, 250,75]);
                image.rotate(45, [50, 100, 250, 75], done);
            });
        });

        describe('-5 degs, {r:200,g:110,b:220} fill', () => {
            it('should succeed', done => {
                current.push(-5, 'degs', 'r-200,g-110,b-220');
                image.rotate(-5, {
                    r: 200,
                    g: 110,
                    b: 220
                }, done);
            });
        });

        describe('-5 degs, {r:200,g:110,b:220,a:50} fill', () => {
            it('should succeed', done => {
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
