(function(undefined) {

    var path = require('path'),
        fs = require('fs'),
        decree = require('decree'),
        defs = require('./defs'),
        util = require('./util'),
        decoder = require('../build/Release/lwip_decoder'),
        Image = require('./Image');

    var openers = [{
        exts: ['jpg', 'jpeg'],
        opener: decoder.jpeg
    }, {
        exts: ['png'],
        opener: decoder.png
    }];

    function open() {
        decree(defs.args.open)(arguments, function(source, type, callback) {
            if (typeof source === 'string') {
                type = type || path.extname(source).slice(1);
                var opener = getOpener(type);
                fs.readFile(source, function(err, imbuff) {
                    if (err) return callback(err);
                    opener(imbuff, function(err, pixelsBuf, width, height, channels, trans) {
                        callback(err, err ? null : new Image(pixelsBuf, width, height, trans));
                    });
                });
            } else if (source instanceof Buffer) {
                var opener = getOpener(type);
                opener(source, function(err, pixelsBuf, width, height, channels, trans) {
                    callback(err, err ? null : new Image(pixelsBuf, width, height, trans));
                });
            } else throw Error("Invalid source");
        });
    }

    function getOpener(ext) {
        ext = ext.toLowerCase();
        for (var i = 0; i < openers.length; i++) {
            var opener = openers[i].opener,
                exts = openers[i].exts;
            if (exts.indexOf(ext) !== -1) return opener;
        }
        throw Error('Unknown type \'' + ext + '\'');
    }

    function create() {
        decree(defs.args.create)(arguments, function(width, height, color, callback) {
            color = util.normalizeColor(color);
            var trans = color.a < 100,
                c_len = width * height,
                pixelsBuf = new Buffer(c_len * 4);
            for (var i = 0; i < width * height; i++) {
                pixelsBuf[i] = color.r;
                pixelsBuf[c_len + i] = color.g;
                pixelsBuf[2 * c_len + i] = color.b;
                pixelsBuf[3 * c_len + i] = color.a;
            }
            setImmediate(function() {
                callback(null, new Image(pixelsBuf, width, height, trans));
            });
        });
    }

    // EXPORTS
    // -------
    module.exports = {
        open: open,
        create: create
    };
})(void 0);
