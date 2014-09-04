// methods should throw errors when arguments are invalid

var should = require("should"),
    lwip = require('../../'),
    imgs = require('../imgs');

describe('lwip.open arguments validation', function() {
    describe('path without extension and no type', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip, imgs.jpg.noex, function() {}).should.throwError();
        });
    });
    describe('path with unknown extension and no type', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip, imgs.inv, function() {}).should.throwError();
        });
    });
    describe('invalid type', function() {
        it('should throw an error', function() {
            lwip.open.bind(lwip, imgs.jpg.rgb, 'jjpg', function() {}).should.throwError();
        });
    });
});
