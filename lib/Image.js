(function(undefined) {

    var path = require('path'),
        fs = require('fs'),
        decree = require('decree'),
        defs = require('./defs'),
        util = require('./util'),
        encoder = require('../build/Release/lwip_encoder'),
        lwip_image = require('../build/Release/lwip_image'),
        normalizeColor = util.normalizeColor;

    var judges = {
        scale: decree(defs.args.scale),
        resize: decree(defs.args.resize),
        rotate: decree(defs.args.rotate),
        blur: decree(defs.args.blur),
        hslaAdjust: decree(defs.args.hslaAdjust),
        saturate: decree(defs.args.saturate),
        lighten: decree(defs.args.lighten),
        darken: decree(defs.args.darken),
        fade: decree(defs.args.fade),
        opacify: decree(defs.args.opacify),
        hue: decree(defs.args.hue),
        crop: decree(defs.args.crop),
        mirror: decree(defs.args.mirror),
        pad: decree(defs.args.pad),
        border: decree(defs.args.border),
        sharpen: decree(defs.args.sharpen),
        paste: decree(defs.args.paste),
        clone: decree(defs.args.clone),
        extract: decree(defs.args.extract),
        toBuffer: decree(defs.args.toBuffer),
        writeFile: decree(defs.args.writeFile),
        setPixel: decree(defs.args.setPixel)
    };

    function Image(pixelsBuf, width, height, trans) {
        this.__lwip = new lwip_image.LwipImage(pixelsBuf, width, height);
        this.__locked = false;
        this.__trans = trans;
    }

    Image.prototype.__lock = function() {
        if (!this.__locked) this.__locked = true;
        else throw Error("Another image operation already in progress");
    };

    Image.prototype.__release = function() {
        this.__locked = false;
    };

    Image.prototype.width = function() {
        return this.__lwip.width();
    };

    Image.prototype.height = function() {
        return this.__lwip.height();
    };

    Image.prototype.size = function() {
        return {
            width: this.__lwip.width(),
            height: this.__lwip.height()
        };
    };

    Image.prototype.scale = function() {
        this.__lock();
        var that = this;
        judges.scale(
            arguments,
            function(wRatio, hRatio, inter, callback) {
                if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
                hRatio = hRatio || wRatio;
                var width = +wRatio * that.width(),
                    height = +hRatio * that.height();
                that.__lwip.resize(width, height, defs.interpolations[inter], function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.resize = function() {
        this.__lock();
        var that = this;
        judges.resize(
            arguments,
            function(width, height, inter, callback) {
                if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
                height = height || width;
                that.__lwip.resize(+width, +height, defs.interpolations[inter], function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.rotate = function() {
        this.__lock();
        var that = this;
        judges.rotate(
            arguments,
            function(degs, color, callback) {
                color = normalizeColor(color);
                if (color.a < 100) that.__trans = true;
                that.__lwip.rotate(+degs, +color.r, +color.g, +color.b, +color.a, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.blur = function() {
        this.__lock();
        var that = this;
        judges.blur(
            arguments,
            function(sigma, callback) {
                that.__lwip.blur(+sigma, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.hslaAdjust = function() {
        this.__lock();
        var that = this;
        judges.hslaAdjust(
            arguments,
            function(hs, sd, ld, ad, callback) {
                that.__lwip.hslaAdj(+hs, +sd, +ld, +ad, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.saturate = function() {
        this.__lock();
        var that = this;
        judges.saturate(
            arguments,
            function(delta, callback) {
                that.__lwip.hslaAdj(0, +delta, 0, 0, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.lighten = function() {
        this.__lock();
        var that = this;
        judges.lighten(
            arguments,
            function(delta, callback) {
                that.__lwip.hslaAdj(0, 0, +delta, 0, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.darken = function() {
        this.__lock();
        var that = this;
        judges.darken(
            arguments,
            function(delta, callback) {
                that.__lwip.hslaAdj(0, 0, -delta, 0, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.fade = function() {
        this.__lock();
        var that = this;
        judges.fade(
            arguments,
            function(delta, callback) {
                that.__lwip.hslaAdj(0, 0, 0, -delta, function(err) {
                    if (+delta > 0) that.__trans = true;
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.opacify = function() {
        this.__lock();
        var that = this;
        judges.opacify(
            arguments,
            function(callback) {
                that.__lwip.opacify(function(err) {
                    that.__trans = false;
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.hue = function() {
        this.__lock();
        var that = this;
        judges.hue(
            arguments,
            function(shift, callback) {
                that.__lwip.hslaAdj(+shift, 0, 0, 0, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.crop = function() {
        this.__lock();
        var that = this;
        judges.crop(
            arguments,
            function(left, top, right, bottom, callback) {
                if (!right && !bottom) {
                    var size = that.size(),
                        width = left,
                        height = top;
                    left = 0 | (size.width - width) / 2;
                    top = 0 | (size.height - height) / 2;
                    right = left + width - 1;
                    bottom = top + height - 1;
                }
                that.__lwip.crop(left, top, right, bottom, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.mirror = function() {
        this.__lock();
        var that = this;
        judges.mirror(
            arguments,
            function(axes, callback) {
                var xaxis = false,
                    yaxis = false;
                axes = axes.toLowerCase();
                if ('x' === axes) xaxis = true;
                if ('y' === axes) yaxis = true;
                if ('xy' === axes || 'yx' === axes) {
                    xaxis = true;
                    yaxis = true;
                }
                if (!(xaxis || yaxis)) {
                    that.__release();
                    throw Error('Invalid axes');
                }
                that.__lwip.mirror(xaxis, yaxis, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    // mirror alias:
    Image.prototype.flip = Image.prototype.mirror;

    Image.prototype.pad = function() {
        this.__lock();
        var that = this;
        judges.pad(
            arguments,
            function(left, top, right, bottom, color, callback) {
                color = normalizeColor(color);
                if (color.a < 100) that.__trans = true;
                that.__lwip.pad(+left, +top, +right, +bottom, +color.r, +color.g, +color.b, +color.a, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.border = function() {
        this.__lock();
        var that = this;
        judges.border(
            arguments,
            function(width, color, callback) {
                color = normalizeColor(color);
                if (color.a < 100) that.__trans = true;
                // we can just use image.pad...
                that.__lwip.pad(+width, +width, +width, +width, +color.r, +color.g, +color.b, +color.a, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.sharpen = function() {
        this.__lock();
        var that = this;
        judges.sharpen(
            arguments,
            function(amplitude, callback) {
                that.__lwip.sharpen(+amplitude, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.paste = function() {
        this.__lock();
        var that = this;
        judges.paste(
            arguments,
            function(left, top, img, callback) {
                if (!(img instanceof Image))
                    throw Error("Pasted image is not a valid Image object");
                // first we retrieve what we need (buffer, dimensions, ...)
                // synchronously so that the pasted image doesn't have a chance
                // to be changed
                var pixbuff = img.__lwip.buffer(),
                    width = img.__lwip.width(),
                    height = img.__lwip.height();
                if (left + width > that.__lwip.width() || top + height > that.__lwip.height())
                    throw Error("Pasted image exceeds dimensions of base image");
                that.__lwip.paste(+left, +top, pixbuff, +width, +height, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.setPixel = function() {
        this.__lock();
        var that = this;
        judges.setPixel(
            arguments,
            function(left, top, color, callback) {
                color = normalizeColor(color);
                if (color.a < 100) that.__trans = true;
                that.__lwip.setPixel(+left, +top, +color.r, +color.g, +color.b, +color.a, function(err) {
                    that.__release();
                    callback(err, that);
                });
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.clone = function() {
        // no need to lock the image. we don't modify the memory buffer.
        // just copy it.
        var that = this;
        judges.clone(
            arguments,
            function(callback) {
                // first we retrieve what we need (buffer, dimensions, ...)
                // synchronously so that the original image doesn't have a chance
                // to be changed (remember, we don't lock it); and only then call
                // the callback asynchronously.
                var pixbuff = that.__lwip.buffer(),
                    width = that.__lwip.width(),
                    height = that.__lwip.height(),
                    trans = that.__trans;
                setImmediate(function() {
                    callback(null, new Image(pixbuff, width, height, trans));
                });
            }
        );
    };

    Image.prototype.extract = function() {
        // no need to lock the image. we don't modify the memory buffer.
        // just copy it and then crop it.
        var that = this;
        judges.extract(
            arguments,
            function(left, top, right, bottom, callback) {
                // first we retrieve what we need (buffer, dimensions, ...)
                // synchronously so that the original image doesn't have a chance
                // to be changed (remember, we don't lock it); then we crop it and
                // only call the callback asynchronously.
                var pixbuff = that.__lwip.buffer(),
                    width = that.__lwip.width(),
                    height = that.__lwip.height(),
                    trans = that.__trans,
                    eximg = new Image(pixbuff, width, height, trans);
                eximg.__lwip.crop(left, top, right, bottom, function(err) {
                    callback(err, eximg);
                });
            }
        );
    };

    Image.prototype.toBuffer = function() {
        this.__lock();
        var that = this;
        judges.toBuffer(
            arguments,
            function(type, params, callback) {
                if (type === 'jpg' || type === 'jpeg') {
                    if (params.quality != 0)
                        params.quality = params.quality || defs.defaults.DEF_JPEG_QUALITY;
                    if (params.quality != parseInt(params.quality) || params.quality < 0 || params.quality > 100)
                        throw Error('Invalid JPEG quality');
                    return encoder.jpeg(
                        that.__lwip.buffer(),
                        that.__lwip.width(),
                        that.__lwip.height(),
                        params.quality,
                        function(err, buffer) {
                            that.__release();
                            callback(err, buffer);
                        }
                    );
                } else if (type === 'png') {
                    params.compression = params.compression || defs.defaults.PNG_DEF_COMPRESSION;
                    if (params.compression === 'none') params.compression = 0;
                    else if (params.compression === 'fast') params.compression = 1;
                    else if (params.compression === 'high') params.compression = 2;
                    else throw Error('Invalid PNG compression');
                    params.interlaced = params.interlaced || defs.defaults.PNG_DEF_INTERLACED;
                    if (typeof params.interlaced !== 'boolean') throw Error('PNG \'interlaced\' must be boolean');
                    params.transparency = params.transparency || defs.defaults.PNG_DEF_TRANSPARENT;
                    if (typeof params.transparency !== 'boolean'){
                        if (typeof params.transparency === 'string' && params.transparency.toLowerCase() === 'auto')
                            params.transparency = that.__trans;
                        else throw Error('PNG \'transparency\' must be boolean or \'auto\'');
                    }
                    return encoder.png(
                        that.__lwip.buffer(),
                        that.__lwip.width(),
                        that.__lwip.height(),
                        params.compression,
                        params.interlaced,
                        params.transparency,
                        function(err, buffer) {
                            that.__release();
                            callback(err, buffer);
                        }
                    );
                } else throw Error('Unknown type \'' + type + '\'');
            },
            function(err) {
                that.__release();
                throw err;
            }
        );
    };

    Image.prototype.writeFile = function() {
        var that = this;
        judges.writeFile(
            arguments,
            function(outpath, type, params, callback) {
                type = type || path.extname(outpath).slice(1).toLowerCase();
                that.toBuffer(type, params, function(err, buffer) {
                    if (err) return callback(err);
                    fs.writeFile(outpath, buffer, {
                        encoding: 'binary'
                    }, callback);
                });
            }
        );
    };

    // EXPORTS
    // -------
    module.exports = Image;

})(void 0);
