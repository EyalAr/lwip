// methods should throw errors when arguments are invalid

var should = require("should"),
    fs = require('fs'),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.open arguments validation', function() {

    describe('invalid source', function() {

        describe('with type', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, 123, 'jpg', function() {}).should.throwError();
            });
        });
        describe('without type', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, {}, function() {}).should.throwError();
            });
        });

    });

    describe('path', function() {

        describe('without extension and no type', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, imgs.jpg.noex, function() {}).should.throwError();
            });
        });
        describe('with unknown extension and no type', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, imgs.inv, function() {}).should.throwError();
            });
        });
        describe('with invalid type', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, imgs.jpg.rgb, 'jjpg', function() {}).should.throwError();
            });
        });

    });

    describe('buffer', function() {

        var buffer;
        before(function(done) {
            fs.readFile(imgs.jpg.rgb, function(err, buff) {
                buffer = buff;
                done();
            });
        });

        describe('without type', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, function() {}).should.throwError();
            });
        });
        describe('with invalid type', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, 'jjpg', function() {}).should.throwError();
            });
        });

    });

});
