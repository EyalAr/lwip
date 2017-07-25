// methods should throw errors when arguments are invalid

const should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('batch.writeFile arguments validation', () => {

    let batch;
    before(done => {
        lwip.open(imgs.jpg.rgb, (err, img) => {
            batch = img.batch().blur(2);
            done(err);
        });
    });

    describe('invalid encoding format', () => {
        it('should throw an error', () => {
            batch.writeFile.bind(batch, 'res.jpg', 'jjpeg').should.throwError();
        });
    });

    describe('JPEG params', () => {

        describe('invalid quality', () => {
            it('should throw an error', () => {
                batch.writeFile.bind(batch, 'res.jpg', 'jpeg', {
                    quality: 'foo'
                }).should.throwError();
            });
        });

    });

    describe('PNG params', () => {

        describe('invalid compression', () => {
            it('should throw an error', () => {
                batch.writeFile.bind(batch, 'res.png', 'png', {
                    compression: 'foo'
                }, () => {}).should.throwError();
            });
        });

        describe('invalid interlaced', () => {
            it('should throw an error', () => {
                batch.writeFile.bind(batch, 'res.png', 'png', {
                    interlaced: 'foo'
                }, () => {}).should.throwError();
            });
        });

    });

});
