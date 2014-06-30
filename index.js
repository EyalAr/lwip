(function(undefined) {

    var DEF_JPEG_QUALITY = 100,
        DEF_INTERPOLATION = 'lanczos';

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

    image.prototype.rotate = function(degs, callback) {
        degs = +degs;
        var that = this;
        this.__lwip.rotate(degs, 255, 255, 255, function(err) {
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
