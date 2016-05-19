(function(undefined) {

    var path = require('path'),
        fs = require('fs'),
        decree = require('decree'),
        defs = require('./defs'),
        util = require('./util'),
        decoder = require('bindings')('lwip_decoder'),
        Image = require('./Image');

    var openers = [{
        exts: ['jpg', 'jpeg'],
        opener: decoder.jpeg
    }, {
        exts: ['png'],
        opener: decoder.png
    }, {
        exts: ['gif'],
        opener: decoder.gif
    }];

    function open() {
        decree(defs.args.open)(arguments, function(source, type, callback) {
            var opener;
            if (typeof source === 'string') {
                type = type || path.extname(source).slice(1);
                if (typeof type !== 'string') throw Error('Invalid type');
                opener = getOpener(type);
                fs.readFile(source, function(err, imbuff) {
                    if (err) return callback(err);
                    opener(imbuff, function(err, pixelsBuf, width, height, channels, trans, metadata) {
                        callback(err, err ? null : new Image(pixelsBuf, width, height, trans, metadata));
                    });
                });
            } else if (source instanceof Buffer) {
                if (typeof type === 'object') {
                    // it's a raw pixels buffer
                    var channelSize = type.width * type.height;
                    var numChannels = source.length / channelSize;
                    if (numChannels !== parseInt(numChannels) || numChannels < 1 || numChannels > 4)
                        throw Error("Buffer size does not match width and height");
                    if (numChannels !== 4) source = util.makeRgbaBuffer(source, numChannels);
                    callback(null, new Image(source, type.width, type.height, (numChannels % 2 === 0)));
                } else if (typeof type === 'string') {
                    // it's an encoded image
                    opener = getOpener(type);
                    opener(source, function(err, pixelsBuf, width, height, channels, trans, metadata) {
                        callback(err, err ? null : new Image(pixelsBuf, width, height, trans, metadata));
                    });
                } else throw Error('Invalid type');
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
