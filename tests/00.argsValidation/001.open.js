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
        describe('with invalid type (raw buffer properties)', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, imgs.jpg.rgb, {width: 120, height: 120}, function() {}).should.throwError();
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
            buffer = new Buffer(120 * 120);
            done();
        });

        describe('without raw buffer properties', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, function() {}).should.throwError();
            });
        });

        describe('without width', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { height: 120 }, function() {}).should.throwError();
            });
        });

        describe('without height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: 120 }, function() {}).should.throwError();
            });
        });

        describe('without width and height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { }, function() {}).should.throwError();
            });
        });

        describe('with non numeric width', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: "lorem", height: 120 }, function() {}).should.throwError();
            });
        });

        describe('with non numeric height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: 120, height: "lorem" }, function() {}).should.throwError();
            });
        });

        describe('with non numeric width and height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: "lorem", height: "ipsum" }, function() {}).should.throwError();
            });
        });

        describe('with negative width', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: -120, height: 120 }, function() {}).should.throwError();
            });
        });

        describe('with negative height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: 120, height: -120 }, function() {}).should.throwError();
            });
        });

        describe('with negative width and height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: -120, height: -120 }, function() {}).should.throwError();
            });
        });

        describe('with incorrect width and height', function() {
            it('should throw an error', function() {
                lwip.open.bind(lwip, buffer, { width: 123, height: 321 }, function() {}).should.throwError();
            });
        });

        describe('with correct width and height for 1 channel', function() {
            it('should succeed', function() {
                lwip.open.bind(lwip, buffer, { width: 120, height: 120 }, function() {}).should.not.throw();
            });
        });

        describe('with correct width and height for 2 channels', function() {
            var newBuffer = new Buffer(120 * 120 * 2);
            it('should succeed', function() {
                lwip.open.bind(lwip, newBuffer, { width: 120, height: 120 }, function() {}).should.not.throw();
            });
        });

        describe('with correct width and height for 3 channels', function() {
            var newBuffer = new Buffer(120 * 120 * 3);
            it('should succeed', function() {
                lwip.open.bind(lwip, newBuffer, { width: 120, height: 120 }, function() {}).should.not.throw();
            });
        });

        describe('with correct width and height for 4 channels', function() {
            var newBuffer = new Buffer(120 * 120 * 4);
            it('should succeed', function() {
                lwip.open.bind(lwip, newBuffer, { width: 120, height: 120 }, function() {}).should.not.throw();
            });
        });

    });

});
