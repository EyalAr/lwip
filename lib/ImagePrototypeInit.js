(function(undefined) {

    var Image = require('./Image'),
        path = require('path'),
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
        contain: decree(defs.args.contain),
        cover: decree(defs.args.cover),
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
        setPixel: decree(defs.args.setPixel),
        getPixel: decree(defs.args.getPixel)
    };

    Image.prototype.__lock = function() {
        if (!this.__locked) this.__locked = true;
        else throw Error("Another image operation already in progress");
    };

    Image.prototype.__release = function() {
        this.__locked = false;
    };

    Image.prototype.getMetadata = function() {
        return this.__metadata;
    };

    Image.prototype.setMetadata = function(data) {
        if (typeof data != "string" && data != null) {
            throw Error("Metadata must be a string or null");
        }

        this.__metadata = data;
        return data;
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

    Image.prototype.getPixel = function() {
        var args = judges.getPixel(arguments),
            left = args[0],
            top = args[1];

        if (left >= this.width() || top >= this.height())
            throw Error("Coordinates exceed dimensions of image");

        var rgba = this.__lwip.getPixel(left, top);

        return {
            r: rgba[0],
            g: rgba[1],
            b: rgba[2],
            a: rgba[3]
        };
    };

    Image.prototype.scale = function() {
        this.__lock();
        var that = this;
        judges.scale(
            arguments,
            function(wRatio, hRatio, inter, callback) {
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

    Image.prototype.contain = function() {
        var that = this;
        judges.contain(
            arguments,
            function(width, height, color, inter, callback) {
                var s = Math.min(width / that.width(), height / that.height());
                that.scale(s, s, inter, function(err){
                    if (err) return callback(err);
                    var padX = (width - that.width()) / 2,
                        padY = (height - that.height()) / 2;
                    that.pad(
                        Math.ceil(padX),
                        Math.ceil(padY),
                        Math.floor(padX),
                        Math.floor(padY),
                        color,
                        callback
                    );
                });
            }
        );
    };

    Image.prototype.cover = function() {
        var that = this;
        judges.cover(
            arguments,
            function(width, height, inter, callback) {
                var s = Math.max(width / that.width(), height / that.height());
                that.scale(s, s, inter, function(err){
                    if (err) return callback(err);
                    that.crop(width, height, callback);
                });
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
                if ('x' === axes) xaxis = true;
                if ('y' === axes) yaxis = true;
                if ('xy' === axes || 'yx' === axes) {
                    xaxis = true;
                    yaxis = true;
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
        try{
            judges.paste(
                arguments,
                function(left, top, img, callback) {
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
                }
            );
        } catch(err){
            that.__release();
            throw err;
        }
    };

    Image.prototype.setPixel = function() {
        this.__lock();
        var that = this;
        try{
            judges.setPixel(
                arguments,
                function(left, top, color, callback) {
                    if (left >= that.width() || top >= that.height())
                        throw Error("Coordinates exceed dimensions of image");
                    color = normalizeColor(color);
                    if (color.a < 100) that.__trans = true;
                    that.__lwip.setPixel(+left, +top, +color.r, +color.g, +color.b, +color.a, function(err) {
                        that.__release();
                        callback(err, that);
                    });
                }
            );
        } catch(err){
            that.__release();
            throw err;
        }
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
        try{
            judges.toBuffer(
                arguments,
                function(type, params, callback) {
                    if (type === 'jpg' || type === 'jpeg') {
                        util.normalizeJpegParams(params);
                        return encoder.jpeg(
                            that.__lwip.buffer(),
                            that.__lwip.width(),
                            that.__lwip.height(),
                            params.quality,
                            encoderCb
                        );
                    } else if (type === 'png') {
                        util.normalizePngParams(params);
                        return encoder.png(
                            that.__lwip.buffer(),
                            that.__lwip.width(),
                            that.__lwip.height(),
                            params.compression,
                            params.interlaced,
                            params.transparency === 'auto' ? that.__trans : params.transparency,
                            that.__metadata,
                            encoderCb
                        );
                    } else if (type === 'gif') {
                        util.normalizeGifParams(params);
                        return encoder.gif(
                            that.__lwip.buffer(),
                            that.__lwip.width(),
                            that.__lwip.height(),
                            util.getClosest2Exp(params.colors),
                            params.colors,
                            params.interlaced,
                            params.transparency === 'auto' ? that.__trans : params.transparency,
                            params.threshold,
                            encoderCb
                        );
                    } else throw Error('Unknown type \'' + type + '\'');

                    function encoderCb(err, buffer) {
                        that.__release();
                        callback(err, buffer);
                    }
                }
            );
        } catch (err){
            that.__release();
            throw err;
        }
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

})(void 0);
