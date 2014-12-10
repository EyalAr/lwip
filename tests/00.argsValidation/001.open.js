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

    describe('pixelbuffer', function() {

        var buffer;
        before(function(done) {
            buffer = new Buffer(100 * 100 * 4);
            done();
        });

        describe('without width', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { height: 100 }, function() {}).should.throwError();
            });
        });

        describe('without height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: 100 }, function() {}).should.throwError();
            });
        });

        describe('without width and height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { }, function() {}).should.throwError();
            });
        });

        describe('with non numeric width', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: "lorem", height: 100 }, function() {}).should.throwError();
            });
        });

        describe('with non numeric height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: 100, height: "lorem" }, function() {}).should.throwError();
            });
        });

        describe('with non numeric width and height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: "lorem", height: "ipsum" }, function() {}).should.throwError();
            });
        });

        describe('with incorrect width and height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: 123, height: 321 }, function() {}).should.throwError();
            });
        });

    });

});
