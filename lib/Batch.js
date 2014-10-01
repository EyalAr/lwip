(function(undefined) {

    var path = require('path'),
        fs = require('fs'),
        async = require('async'),
        decree = require('decree'),
        defs = require('./defs'),
        util = require('./util'),
        Image = require('./Image');

    // Extend Image with image.batch()
    Image.prototype.batch = function() {
        return new Batch(this);
    }

    var undefinedFilter = util.undefinedFilter,
        normalizeColor = util.normalizeColor;

    function Batch(image) {
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

    Batch.prototype.exec = function(callback) {
        var that = this;
        if (that.__running) throw Error("Batch is already running");
        that.__running = true;
        async.eachSeries(this.__queue, function(op, done) {
            op.args.push(done);
            // if an exception is thrown here, it should be caught (because we
            // are in the middle of async process) and translated to an 'err'
            // parameter.
            try {
                op.handle.apply(that.__image, op.args);
            } catch (e) {
                done(e);
            }
        }, function(err) {
            that.__queue.length = 0; // queue is now empty
            that.__running = false;
            callback(err, that.__image);
        });
    }

    Batch.prototype.scale = function() {
        var that = this,
            decs = defs.args.scale.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(wRatio, hRatio, inter) {
            if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
            that.__addOp(that.__image.scale, [wRatio, hRatio, inter].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.resize = function() {
        var that = this,
            decs = defs.args.resize.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(width, height, inter) {
            if (!defs.interpolations[inter]) throw Error("Unknown interpolation " + inter);
            that.__addOp(that.__image.resize, [width, height, inter].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.rotate = function() {
        var that = this,
            decs = defs.args.rotate.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(degs, color) {
            color = normalizeColor(color);
            that.__addOp(that.__image.rotate, [degs, color].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.blur = function() {
        var that = this,
            decs = defs.args.blur.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(sigma) {
            that.__addOp(that.__image.blur, [sigma].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.hslaAdjust = function() {
        var that = this,
            decs = defs.args.hslaAdjust.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(hs, sd, ld, ad) {
            that.__addOp(that.__image.hslaAdjust, [hs, sd, ld, ad].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.saturate = function() {
        var that = this,
            decs = defs.args.saturate.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(delta) {
            that.__addOp(that.__image.saturate, [delta].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.lighten = function() {
        var that = this,
            decs = defs.args.lighten.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(delta) {
            that.__addOp(that.__image.lighten, [delta].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.darken = function() {
        var that = this,
            decs = defs.args.darken.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(delta) {
            that.__addOp(that.__image.darken, [delta].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.fade = function() {
        var that = this,
            decs = defs.args.fade.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(delta) {
            that.__addOp(that.__image.fade, [delta].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.opacify = function() {
        this.__addOp(this.__image.opacify, []);
        return this;
    }

    Batch.prototype.hue = function() {
        var that = this,
            decs = defs.args.hue.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(shift) {
            that.__addOp(that.__image.hue, [shift].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.crop = function() {
        var that = this,
            decs = defs.args.crop.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(left, top, right, bottom) {
            that.__addOp(that.__image.crop, [left, top, right, bottom].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.mirror = function() {
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
    Batch.prototype.flip = Batch.prototype.mirror;

    Batch.prototype.pad = function() {
        var that = this,
            decs = defs.args.pad.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(left, top, right, bottom, color) {
            color = normalizeColor(color);
            that.__addOp(that.__image.pad, [left, top, right, bottom, color].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.border = function() {
        var that = this,
            decs = defs.args.border.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(width, color) {
            color = normalizeColor(color);
            that.__addOp(that.__image.border, [width, color].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.sharpen = function() {
        var that = this,
            decs = defs.args.sharpen.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(amplitude) {
            that.__addOp(that.__image.sharpen, [amplitude].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.paste = function() {
        var that = this,
            decs = defs.args.paste.slice(0, -1); // cut callback declaration
        decree(decs)(arguments, function(left, top, img) {
            if (!(img instanceof Image))
                throw Error("Pasted image is not a valid Image object");
            that.__addOp(that.__image.paste, [left, top, img].filter(undefinedFilter));
        });
        return this;
    }

    Batch.prototype.toBuffer = function() {
        var that = this;
        decree(defs.args.toBuffer)(arguments, function(type, params, callback) {
            if (type === 'jpg' || type === 'jpeg') {
                if (params.quality != 0)
                    params.quality = params.quality || defs.defaults.DEF_JPEG_QUALITY;
                if (params.quality != parseInt(params.quality) || params.quality < 0 || params.quality > 100)
                    throw Error('Invalid JPEG quality');
            } else if (type === 'png') {
                params.compression = params.compression || defs.defaults.PNG_DEF_COMPRESSION;
                if (['none', 'fast', 'high'].indexOf(params.compression) === -1)
                    throw Error('Invalid PNG compression');
                params.interlaced = params.interlaced || defs.defaults.PNG_DEF_INTERLACED;
                if (typeof params.interlaced !== 'boolean') throw Error('PNG \'interlaced\' must be boolean');
                params.transparency = params.transparency || defs.defaults.PNG_DEF_TRANSPARENT;
                if (typeof params.transparency !== 'boolean' && params.transparency.toLowerCase() !== 'auto')
                    throw Error('PNG \'transparency\' must be boolean or \'auto\'');
            } else throw Error('Unknown type \'' + type + '\'');
            that.exec(function(err, image) {
                if (err) return callback(err);
                image.toBuffer(type, params, callback);
            });
        });
    }

    Batch.prototype.writeFile = function(outpath, type, params, callback) {
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

    // EXPORTS
    // -------
    module.exports = Batch;

})(void 0);
