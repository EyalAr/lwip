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
                if(typeof type === 'object') {
                    if(!type.width) {
                        throw Error("Missing width");
                    }
                    if(!type.height) {
                        throw Error("Missing height");
                    }
                    if(typeof type.width !== "number") {
                        throw Error("Width must be numeric");
                    }
                    if(typeof type.height !== "number") {
                        throw Error("Height must be numeric");
                    }
                    var channelSize = type.width * type.height;
                    var numChannels = 3;
                    var size = channelSize * numChannels;
                    var bufferSize = source.length;
                    if(size !== bufferSize) {
                        numChannels++;
                        size += channelSize;
                    }
                    if(size !== bufferSize) {
                        throw Error("Invalid width or height");
                    }
                    callback(null, new Image(source, type.width, type.height, (numChannels === 4)));
                } else {
                    var opener = getOpener(type);
                    opener(source, function(err, pixelsBuf, width, height, channels, trans) {
                        callback(err, err ? null : new Image(pixelsBuf, width, height, trans));
                    });
                }
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
