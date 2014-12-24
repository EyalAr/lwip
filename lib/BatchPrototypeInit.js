(function(undefined) {

    var path = require('path'),
        fs = require('fs'),
        async = require('async'),
        decree = require('decree'),
        defs = require('./defs'),
        util = require('./util'),
        Batch = require('./Batch'),
        Image = require('./Image');

    var judges = {
        // slice(0,-1) cuts the callback declaration, which is not needed in
        // batch mode.
        scale: decree(defs.args.scale.slice(0, -1)),
        resize: decree(defs.args.resize.slice(0, -1)),
        contain: decree(defs.args.contain.slice(0, -1)),
        cover: decree(defs.args.cover.slice(0, -1)),
        rotate: decree(defs.args.rotate.slice(0, -1)),
        blur: decree(defs.args.blur.slice(0, -1)),
        hslaAdjust: decree(defs.args.hslaAdjust.slice(0, -1)),
        saturate: decree(defs.args.saturate.slice(0, -1)),
        lighten: decree(defs.args.lighten.slice(0, -1)),
        darken: decree(defs.args.darken.slice(0, -1)),
        fade: decree(defs.args.fade.slice(0, -1)),
        opacify: decree(defs.args.opacify.slice(0, -1)),
        hue: decree(defs.args.hue.slice(0, -1)),
        crop: decree(defs.args.crop.slice(0, -1)),
        mirror: decree(defs.args.mirror.slice(0, -1)),
        pad: decree(defs.args.pad.slice(0, -1)),
        border: decree(defs.args.border.slice(0, -1)),
        sharpen: decree(defs.args.sharpen.slice(0, -1)),
        paste: decree(defs.args.paste.slice(0, -1)),
        clone: decree(defs.args.clone.slice(0, -1)),
        extract: decree(defs.args.extract.slice(0, -1)),
        setPixel: decree(defs.args.setPixel.slice(0, -1)),
        exec: decree(defs.args.exec),
        toBuffer: decree(defs.args.toBuffer),
        writeFile: decree(defs.args.writeFile)
    };

    var undefinedFilter = util.undefinedFilter,
        normalizeColor = util.normalizeColor;

    // Extend Image with image.batch()
    Image.prototype.batch = function() {
        return new Batch(this);
    };

    Batch.prototype.exec = function() {
        var that = this;
        judges.exec(arguments, function(callback) {
            if (that.__running) throw Error("Batch is already running");
            that.__running = true;
            async.eachSeries(that.__queue, function(op, done) {
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
        });
    };

    Batch.prototype.scale = function() {
        var that = this;
        judges.scale(arguments, function(wRatio, hRatio, inter) {
            that.__addOp(that.__image.scale, [wRatio, hRatio, inter].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.resize = function() {
        var that = this;
        judges.resize(arguments, function(width, height, inter) {
            that.__addOp(that.__image.resize, [width, height, inter].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.contain = function() {
        var that = this;
        judges.contain(arguments, function(width, height, color, inter) {
            that.__addOp(that.__image.contain, [width, height, color, inter].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.cover = function() {
        var that = this;
        judges.cover(arguments, function(width, height, inter) {
            that.__addOp(that.__image.cover, [width, height, inter].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.rotate = function() {
        var that = this;
        judges.rotate(arguments, function(degs, color) {
            color = normalizeColor(color);
            that.__addOp(that.__image.rotate, [degs, color].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.blur = function() {
        var that = this;
        judges.blur(arguments, function(sigma) {
            that.__addOp(that.__image.blur, [sigma].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.hslaAdjust = function() {
        var that = this;
        judges.hslaAdjust(arguments, function(hs, sd, ld, ad) {
            that.__addOp(that.__image.hslaAdjust, [hs, sd, ld, ad].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.saturate = function() {
        var that = this;
        judges.saturate(arguments, function(delta) {
            that.__addOp(that.__image.saturate, [delta].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.lighten = function() {
        var that = this;
        judges.lighten(arguments, function(delta) {
            that.__addOp(that.__image.lighten, [delta].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.darken = function() {
        var that = this;
        judges.darken(arguments, function(delta) {
            that.__addOp(that.__image.darken, [delta].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.fade = function() {
        var that = this;
        judges.fade(arguments, function(delta) {
            that.__addOp(that.__image.fade, [delta].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.opacify = function() {
        this.__addOp(this.__image.opacify, []);
        return this;
    };

    Batch.prototype.hue = function() {
        var that = this;
        judges.hue(arguments, function(shift) {
            that.__addOp(that.__image.hue, [shift].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.crop = function() {
        var that = this;
        judges.crop(arguments, function(left, top, right, bottom) {
            that.__addOp(that.__image.crop, [left, top, right, bottom].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.mirror = function() {
        var that = this;
        judges.mirror(arguments, function(axes) {
            that.__addOp(that.__image.mirror, [axes].filter(undefinedFilter));
        });
        return this;
    };

    // mirror alias:
    Batch.prototype.flip = Batch.prototype.mirror;

    Batch.prototype.pad = function() {
        var that = this;
        judges.pad(arguments, function(left, top, right, bottom, color) {
            color = normalizeColor(color);
            that.__addOp(that.__image.pad, [left, top, right, bottom, color].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.border = function() {
        var that = this;
        judges.border(arguments, function(width, color) {
            color = normalizeColor(color);
            that.__addOp(that.__image.border, [width, color].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.sharpen = function() {
        var that = this;
        judges.sharpen(arguments, function(amplitude) {
            that.__addOp(that.__image.sharpen, [amplitude].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.paste = function() {
        var that = this;
        judges.paste(arguments, function(left, top, img) {
            that.__addOp(that.__image.paste, [left, top, img].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.setPixel = function() {
        var that = this;
        judges.setPixel(arguments, function(left, top, color) {
            that.__addOp(that.__image.setPixel, [left, top, color].filter(undefinedFilter));
        });
        return this;
    };

    Batch.prototype.toBuffer = function() {
        var that = this;
        judges.toBuffer(arguments, function(type, params, callback) {
            if (type === 'jpg' || type === 'jpeg') {
                util.normalizeJpegParams(params);
            } else if (type === 'png') {
                util.normalizePngParams(params);
            } else if (type === 'gif') {
                util.normalizeGifParams(params);
            } else throw Error('Unknown type \'' + type + '\'');
            that.exec(function(err, image) {
                if (err) return callback(err);
                image.toBuffer(type, params, callback);
            });
        });
    };

    Batch.prototype.writeFile = function(outpath, type, params, callback) {
        var that = this;
        judges.writeFile(arguments, function(outpath, type, params, callback) {
            type = type || path.extname(outpath).slice(1).toLowerCase();
            that.toBuffer(type, params, function(err, buffer) {
                if (err) return callback(err);
                fs.writeFile(outpath, buffer, {
                    encoding: 'binary'
                }, callback);
            });
        });
    };

})(void 0);
