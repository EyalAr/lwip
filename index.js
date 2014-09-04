(function(undefined) {

    var path = require('path'),
        fs = require('fs'),
        async = require('async'),
        decree = require('decree'),
        defs = require('./defs'),
        lwip = require('./build/Release/lwip');

    var openers = [{
        exts: ['jpg', 'jpeg'],
        opener: lwip.openJpeg
    }, {
        exts: ['png'],
        opener: lwip.openPng
    }];

    function undefinedFilter(v) {
        return v !== undefined;
    }

    function image(lwipImage) {
        this.__lwip = lwipImage;
        this.__locked = false;
    }

    image.prototype.__lock = function() {
        if (!this.__locked) this.__locked = true;
        else throw Error("Another image operation already in progress");
    };

    image.prototype.__release = function() {
        this.__locked = false;
    };

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

    image.prototype.scale = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.scale)(arguments, function(wRatio, hRatio, inter, callback) {
                if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
                hRatio = hRatio || wRatio;
                var width = +wRatio * that.width(),
                    height = +hRatio * that.height();
                that.__lwip.resize(width, height, defs.interpolations[inter], function(err) {
                    that.__release();
                    callback(err, that);
                });
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.resize = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.resize)(arguments, function(width, height, inter, callback) {
                if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
                height = height || width;
                that.__lwip.resize(+width, +height, defs.interpolations[inter], function(err) {
                    that.__release();
                    callback(err, that);
                });
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.rotate = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.rotate)(arguments, function(degs, color, callback) {
                if (typeof color === 'string') {
                    if (defs.colors[color]) color = defs.colors[color];
                    else throw Error('Unknown color ' + color);
                } else {
                    if (color instanceof Array) {
                        color = {
                            r: color[0],
                            g: color[1],
                            b: color[2]
                        };
                    }
                    if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                        throw Error('\'red\' color component is invalid');
                    if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                        throw Error('\'green\' color component is invalid');
                    if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                        throw Error('\'blue\' color component is invalid');
                }
                that.__lwip.rotate(+degs, +color.r, +color.g, +color.b, function(err) {
                    that.__release();
                    callback(err, that);
                });
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.blur = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.blur)(arguments, function(sigma, callback) {
                that.__lwip.blur(+sigma, function(err) {
                    that.__release();
                    callback(err, that);
                });
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.crop = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.crop)(arguments, function(left, top, right, bottom, callback) {
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
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.mirror = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.mirror)(arguments, function(axes, callback) {
                var xaxis = false,
                    yaxis = false;
                axes = axes.toLowerCase();
                if ('x' === axes) xaxis = true;
                if ('y' === axes) yaxis = true;
                if ('xy' === axes || 'yx' === axes) {
                    xaxis = true;
                    yaxis = true;
                }
                if (!(xaxis || yaxis)) throw Error('Invalid axes');
                that.__lwip.mirror(xaxis, yaxis, function(err) {
                    that.__release();
                    callback(err, that);
                });
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    // mirror alias:
    image.prototype.flip = image.prototype.mirror;

    image.prototype.pad = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.pad)(arguments, function(left, top, right, bottom, color, callback) {
                if (typeof color === 'string') {
                    if (defs.colors[color]) color = defs.colors[color];
                    else throw Error('Unknown color ' + color);
                } else {
                    if (color instanceof Array) {
                        color = {
                            r: color[0],
                            g: color[1],
                            b: color[2]
                        };
                    }
                    if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                        throw Error('\'red\' color component is invalid');
                    if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                        throw Error('\'green\' color component is invalid');
                    if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                        throw Error('\'blue\' color component is invalid');
                }
                that.__lwip.pad(+left, +top, +right, +bottom, +color.r, +color.g, +color.b, function(err) {
                    that.__release();
                    callback(err, that);
                });
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.border = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.border)(arguments, function(width, color, callback) {
                if (typeof color === 'string') {
                    if (defs.colors[color]) color = defs.colors[color];
                    else throw Error('Unknown color ' + color);
                } else {
                    if (color instanceof Array) {
                        color = {
                            r: color[0],
                            g: color[1],
                            b: color[2]
                        };
                    }
                    if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                        throw Error('\'red\' color component is invalid');
                    if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                        throw Error('\'green\' color component is invalid');
                    if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                        throw Error('\'blue\' color component is invalid');
                }
                // we can just use image.pad...
                that.__lwip.pad(+width, +width, +width, +width, +color.r, +color.g, +color.b, function(err) {
                    that.__release();
                    callback(err, that);
                });
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.toBuffer = function() {
        this.__lock();
        try {
            var that = this;
            decree(defs.args.toBuffer)(arguments, function(type, params, callback) {
                if (type === 'jpg' || type === 'jpeg') {
                    params.quality = params.quality || defs.defaults.DEF_JPEG_QUALITY;
                    if (params.quality != parseInt(params.quality) || params.quality < 0 || params.quality > 100)
                        throw Error('Invalid JPEG quality');
                    return that.__lwip.toJpegBuffer(params.quality, function(err, buffer) {
                        that.__release();
                        callback(err, buffer);
                    });
                } else if (type === 'png') {
                    params.compression = params.compression || defs.defaults.PNG_DEF_COMPRESSION;
                    if (params.compression === 'none') params.compression = 0;
                    else if (params.compression === 'fast') params.compression = 1;
                    else if (params.compression === 'high') params.compression = 2;
                    else throw Error('Invalid PNG compression');
                    params.interlaced = params.interlaced || defs.defaults.PNG_DEF_INTERLACED;
                    if (typeof params.interlaced !== 'boolean') throw Error('PNG \'interlaced\' must be boolean');
                    return that.__lwip.toPngBuffer(params.compression, params.interlaced, function(err, buffer) {
                        that.__release();
                        callback(err, buffer);
                    });
                } else throw Error('Unknown type \'' + type + '\'');
            });
        } catch (e) {
            this.__release();
            throw e;
        }
    }

    image.prototype.writeFile = function() {
        var that = this;
        decree(defs.args.writeFile)(arguments, function(outpath, type, params, callback) {
            type = type || path.extname(outpath).slice(1).toLowerCase();
            that.toBuffer(type, params, function(err, buffer) {
                if (err) return callback(err);
                fs.writeFile(outpath, buffer, {
                    encoding: 'binary'
                }, callback);
            });
        });
    }

    image.prototype.batch = function() {
        return new batch(this);
    }

    function batch(image) {
        this.__image = image;
        this.__queue = [];
        this.__running = false;
        this.__addOp = function(handle, args) {
            this.__queue.push({
                handle: handle,
                args: args
            });
        };
    }

    batch.prototype.exec = function(callback) {
        var that = this;
        if (that.__running) throw Error("Batch is already running");
        that.__running = true;
        async.eachSeries(this.__queue, function(op, done) {
            op.args.push(done);
            op.handle.apply(that.__image, op.args);
        }, function(err) {
            that.__queue.length = 0; // queue is now empty
            that.__running = false;
            callback(err, that.__image);
        });
    }

    batch.prototype.scale = function() {
        var that = this,
            decs = defs.args.scale.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(wRatio, hRatio, inter) {
            if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
            that.__addOp(that.__image.scale, [wRatio, hRatio, inter].filter(undefinedFilter));
        });
        return this;
    }

    batch.prototype.resize = function() {
        var that = this,
            decs = defs.args.resize.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(width, height, inter) {
            if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
            that.__addOp(that.__image.resize, [width, height, inter].filter(undefinedFilter));
        });
        return this;
    }

    batch.prototype.rotate = function() {
        var that = this,
            decs = defs.args.rotate.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(degs, color) {
            if (typeof color === 'string') {
                if (defs.colors[color]) color = defs.colors[color];
                else throw Error('Unknown color ' + color);
            } else {
                if (color instanceof Array) {
                    color = {
                        r: color[0],
                        g: color[1],
                        b: color[2]
                    };
                }
                if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                    throw Error('\'red\' color component is invalid');
                if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                    throw Error('\'green\' color component is invalid');
                if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                    throw Error('\'blue\' color component is invalid');
            }
            that.__addOp(that.__image.rotate, [degs, color].filter(undefinedFilter));
        });
        return this;
    }

    batch.prototype.blur = function(sigma) {
        var that = this,
            decs = defs.args.blur.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(sigma) {
            that.__addOp(that.__image.blur, [sigma].filter(undefinedFilter));
        });
        return this;
    }

    batch.prototype.crop = function() {
        var that = this,
            decs = defs.args.crop.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(left, top, right, bottom) {
            that.__addOp(that.__image.crop, [left, top, right, bottom].filter(undefinedFilter));
        });
        return this;
    }

    batch.prototype.mirror = function() {
        var that = this,
            decs = defs.args.mirror.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(axes) {
            axes = axes.toLowerCase();
            if (['x', 'y', 'xy', 'yx'].indexOf(axes) === -1) throw Error('Invalid axes');
            that.__addOp(that.__image.mirror, [axes].filter(undefinedFilter));
        });
        return this;
    }

    // mirror alias:
    batch.prototype.flip = batch.prototype.mirror;

    batch.prototype.pad = function() {
        var that = this,
            decs = defs.args.pad.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(left, top, right, bottom, color) {
            if (typeof color === 'string') {
                if (defs.colors[color]) color = defs.colors[color];
                else throw Error('Unknown color ' + color);
            } else {
                if (color instanceof Array) {
                    color = {
                        r: color[0],
                        g: color[1],
                        b: color[2]
                    };
                }
                if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                    throw Error('\'red\' color component is invalid');
                if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                    throw Error('\'green\' color component is invalid');
                if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                    throw Error('\'blue\' color component is invalid');
            }
            that.__addOp(that.__image.pad, [left, top, right, bottom, color].filter(undefinedFilter));
        });
        return this;
    }

    batch.prototype.border = function() {
        var that = this,
            decs = defs.args.border.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(width, color) {
            if (typeof color === 'string') {
                if (defs.colors[color]) color = defs.colors[color];
                else throw Error('Unknown color ' + color);
            } else {
                if (color instanceof Array) {
                    color = {
                        r: color[0],
                        g: color[1],
                        b: color[2]
                    };
                }
                if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                    throw Error('\'red\' color component is invalid');
                if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                    throw Error('\'green\' color component is invalid');
                if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                    throw Error('\'blue\' color component is invalid');
            }
            that.__addOp(that.__image.border, [width, color].filter(undefinedFilter));
        });
        return this;
    }

    batch.prototype.toBuffer = function() {
        var that = this;
        decree(defs.args.toBuffer)(arguments, function(type, params, callback) {
            if (type === 'jpg' || type === 'jpeg') {
                params.quality = params.quality || defs.defaults.DEF_JPEG_QUALITY;
                if (params.quality != parseInt(params.quality) || params.quality < 0 || params.quality > 100)
                    throw Error('Invalid JPEG quality');
            } else if (type === 'png') {
                params.compression = params.compression || defs.defaults.PNG_DEF_COMPRESSION;
                if (params.compression === 'none') params.compression = 0;
                else if (params.compression === 'fast') params.compression = 1;
                else if (params.compression === 'high') params.compression = 2;
                else throw Error('Invalid PNG compression');
                params.interlaced = params.interlaced || defs.defaults.PNG_DEF_INTERLACED;
                if (typeof params.interlaced !== 'boolean') throw Error('PNG \'interlaced\' must be boolean');
            } else throw Error('Unknown type \'' + type + '\'');
            that.exec(function(err, image) {
                if (err) return callback(err);
                image.toBuffer(type, params, callback);
            });
        });
    }

    batch.prototype.writeFile = function(outpath, type, params, callback) {
        var that = this;
        decree(defs.args.writeFile)(arguments, function(outpath, type, params, callback) {
            type = type || path.extname(outpath).slice(1).toLowerCase();
            that.toBuffer(type, params, function(err, buffer) {
                if (err) return callback(err);
                fs.writeFile(outpath, buffer, {
                    encoding: 'binary'
                }, callback);
            });
        });
    }

    function open() {
        decree(defs.args.open)(arguments, function(impath, type, callback) {
            type = type || path.extname(impath).slice(1);
            getOpener(type)(impath, function(err, lwipImage) {
                callback(err, err ? undefined : new image(lwipImage));
            });
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

    // EXPORTS
    // -------
    module.exports = {
        open: open
    };
})(void 0);
