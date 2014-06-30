(function(undefined) {

    var DEF_JPEG_QUALITY = 100,
        DEF_INTERPOLATION = 'lanczos',
        DEF_ROTATE_COLOR = 'gray';

    var path = require('path'),
        lwip = require('./build/Release/lwip');

    var openers = [{
        exts: ['jpg', 'jpeg'],
        opener: lwip.openJpeg
    }, {
        exts: ['png'],
        opener: lwip.openPng
    }];

    var interpolations = {
        'nearest-neighbor': 1,
        'moving-average': 2,
        'linear': 3,
        'grid': 4,
        'cubic': 5,
        'lanczos': 6
    };

    var colors = {
        'black': {
            r: 0,
            g: 0,
            b: 0
        },
        'white': {
            r: 255,
            g: 255,
            b: 255
        },
        'red': {
            r: 255,
            g: 0,
            b: 0
        },
        'blue': {
            r: 0,
            g: 0,
            b: 255
        },
        'green': {
            r: 0,
            g: 255,
            b: 0
        },
        'cyan': {
            r: 0,
            g: 255,
            b: 255
        },
        'yellow': {
            r: 255,
            g: 255,
            b: 0
        },
        'gray': {
            r: 128,
            g: 128,
            b: 128
        },
        'magenta': {
            r: 255,
            g: 0,
            b: 255
        }
    };

    function image(lwipImage) {
        this.__lwip = lwipImage;
    }

    image.prototype.width = function() {
        return this.__lwip.width();
    }

    image.prototype.height = function() {
        return this.__lwip.height();
    }

    image.prototype.size = function() {
        return {
            width: this.__lwip.width(),
            height: this.__lwip.height()
        };
    }

    image.prototype.scale = function(wRatio, hRatio, inter, callback) {
        if (!wRatio || isNaN(wRatio) || wRatio <= 0)
            throw new TypeError('\'wRatio\' argument must be a positive number');
        callback = callback || inter || hRatio;
        if (typeof callback !== 'function')
            throw new TypeError('\'callback\' argument must be a function');
        if (typeof hRatio === 'function') inter = hRatio = undefined;
        else if (typeof inter === 'function') inter = undefined;
        inter = inter || DEF_INTERPOLATION;
        hRatio = hRatio || wRatio;
        if (!hRatio || isNaN(hRatio) || hRatio <= 0)
            throw new TypeError('\'hRatio\' argument must be a positive number');
        if (!interpolations[inter])
            throw new TypeError('\'inter\' argument must be a valid interpolation string');

        wRatio = +wRatio;
        hRatio = +hRatio;
        var width = wRatio * this.width(),
            height = hRatio * this.height(),
            that = this;
        this.__lwip.resize(width, height, interpolations[inter], function(err) {
            callback(err, that);
        });
    }

    image.prototype.resize = function(width, height, inter, callback) {
        if (!width || isNaN(width) || width <= 0)
            throw new TypeError('\'width\' argument must be a positive number');
        callback = callback || inter || height;
        if (typeof callback !== 'function')
            throw new TypeError('\'callback\' argument must be a function');
        if (typeof height === 'function') inter = height = undefined;
        else if (typeof inter === 'function') inter = undefined;
        inter = inter || DEF_INTERPOLATION;
        height = height || width;
        if (!height || isNaN(height) || height <= 0)
            throw new TypeError('\'height\' argument must be a positive number');
        if (!interpolations[inter])
            throw new TypeError('\'inter\' argument must be a valid interpolation string');

        width = +width;
        height = +height;
        var that = this;
        this.__lwip.resize(width, height, interpolations[inter], function(err) {
            callback(err, that);
        });
    }

    image.prototype.rotate = function(degs, color, callback) {
        if ((!degs && degs != 0) || isNaN(degs))
            throw new TypeError('\'degs\' argument must be a number');
        callback = callback || color;
        if (typeof callback !== 'function')
            throw new TypeError('\'callback\' argument must be a function');
        if (typeof color === 'function') color = DEF_ROTATE_COLOR;
        if (typeof color === 'string') {
            if (colors[color]) color = colors[color];
            else throw new TypeError('\'color\' argument is invalid');
        } else {
            if (color instanceof Array) {
                color = {
                    r: color[0],
                    g: color[1],
                    b: color[2]
                };
            }
            if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                throw new TypeError('\'red\' color component is invalid');
            if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                throw new TypeError('\'green\' color component is invalid');
            if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                throw new TypeError('\'blue\' color component is invalid');
        }

        var that = this;
        this.__lwip.rotate(+degs, +color.r, +color.g, +color.b, function(err) {
            callback(err, that);
        });
    }

    image.prototype.toBuffer = function(type, params, callback) {
        if (typeof type !== 'string')
            throw new TypeError('\'type\' argument must be a string');
        type = type.toLowerCase();
        callback = callback || params;
        params = params && typeof params === 'object' ? params : typeof params === 'function' ? {} : undefined;
        if (typeof params !== 'object')
            throw new TypeError('\'params\' argument must be an object');
        if (typeof callback !== 'function')
            throw new TypeError('\'callback\' argument must be a function');

        if (type === 'jpg' || type === 'jpeg') {
            params.quality = params.quality || DEF_JPEG_QUALITY;
            if (params.quality < 0 || params.quality > 100)
                return setImmediate(function() {
                    callback('Invalid JPEG quality');
                });
            return this.__lwip.toJpegBuffer(params.quality, callback);
        } else setImmediate(function() {
            callback('Unknown type \'' + type + '\'');
        });
    }

    function open(impath, type, callback) {
        if (typeof impath !== 'string')
            throw new TypeError('\'path\' argument must be a string');
        callback = callback || type;
        type = typeof type === 'string' ? type : typeof type === 'function' ? path.extname(impath).slice(1) : undefined;
        if (typeof type !== 'string')
            throw new TypeError('\'type\' argument must be a string');
        if (typeof callback !== 'function')
            throw new TypeError('\'callback\' argument must be a function');

        getOpener(type)(impath, function(err, lwipImage) {
            callback(err, err ? undefined : new image(lwipImage));
        });
    }

    function getOpener(ext) {
        for (var i = 0; i < openers.length; i++) {
            var opener = openers[i].opener,
                exts = openers[i].exts;
            if (exts.indexOf(ext) !== -1) return opener;
        }
        return function(impath, callback) {
            setImmediate(function() {
                callback('Unknown type \'' + ext + '\'');
            });
        }
    }

    // EXPORTS
    // -------
    module.exports = {
        open: open
    };
})(void 0);
