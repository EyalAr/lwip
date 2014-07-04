// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.open arguments validation', function() {
    describe('with no arguments', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip).should.throwError();
        });
    });
    describe('with no callback', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip, imgs.jpg.rgb).should.throwError();
        });
    });
    describe('with path without extension and no type', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip, imgs.jpg.noex, function() {}).should.throwError();
        });
    });
    describe('with path without extension, no type and no callback', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip, imgs.jpg.noex).should.throwError();
        });
    });
    describe('with path, invalid type and callback', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip, imgs.jpg.rgb, 'jjpg', function() {}).should.throwError();
        });
    });
});
