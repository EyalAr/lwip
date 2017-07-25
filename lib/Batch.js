'use strict';

const path = require('path'),
    fs = require('fs'),
    async = require('async'),
    decree = require('decree'),
    defs = require('./defs'),
    util = require('./util'),
    Image = require('./Image');

const judges = {
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


const undefinedFilter = util.undefinedFilter,
    normalizeColor = util.normalizeColor;

class Batch {
    constructor (image) {
        this.__image = image;
        this.__queue = [];
        this.__running = false;
    }

    __addOp (handle, args) {
        this.__queue.push({
            handle: handle,
            args: args
        });
    }

    exec () {
        judges.exec(arguments, callback => {
            if (this.__running) throw Error("Batch is already running");
            this.__running = true;
            async.eachSeries(this.__queue, (op, done) => {
                op.args.push(done);
                // if an exception is thrown here, it should be caught (because we
                // are in the middle of async process) and translated to an 'err'
                // parameter.
                try {
                    op.handle.apply(this.__image, op.args);
                } catch (e) {
                    done(e);
                }
            }, err => {
                this.__queue.length = 0; // queue is now empty
                this.__running = false;
                callback(err, this.__image);
            });
        });
    }

    scale () {
        judges.scale(arguments, (wRatio, hRatio, inter) => {
            this.__addOp(this.__image.scale, [wRatio, hRatio, inter].filter(undefinedFilter));
        });
        return this;
    }

    resize () {
        judges.resize(arguments, (width, height, inter) => {
            this.__addOp(this.__image.resize, [width, height, inter].filter(undefinedFilter));
        });
        return this;
    }

    contain () {
        judges.contain(arguments, (width, height, color, inter) => {
            this.__addOp(this.__image.contain, [width, height, color, inter].filter(undefinedFilter));
        });
        return this;
    }

    cover () {
        judges.cover(arguments, (width, height, inter) => {
            this.__addOp(this.__image.cover, [width, height, inter].filter(undefinedFilter));
        });
        return this;
    }

    rotate () {
        judges.rotate(arguments, (degs, color) => {
            color = normalizeColor(color);
            this.__addOp(this.__image.rotate, [degs, color].filter(undefinedFilter));
        });
        return this;
    }

    blur () {
        judges.blur(arguments, sigma => {
            this.__addOp(this.__image.blur, [sigma].filter(undefinedFilter));
        });
        return this;
    }

    hslaAdjust () {
        judges.hslaAdjust(arguments, (hs, sd, ld, ad) => {
            this.__addOp(this.__image.hslaAdjust, [hs, sd, ld, ad].filter(undefinedFilter));
        });
        return this;
    }

    saturate () {
        judges.saturate(arguments, delta => {
            this.__addOp(this.__image.saturate, [delta].filter(undefinedFilter));
        });
        return this;
    }

    lighten () {
        judges.lighten(arguments, delta => {
            this.__addOp(this.__image.lighten, [delta].filter(undefinedFilter));
        });
        return this;
    }

    darken () {
        judges.darken(arguments, delta => {
            this.__addOp(this.__image.darken, [delta].filter(undefinedFilter));
        });
        return this;
    }

    fade () {
        judges.fade(arguments, delta => {
            this.__addOp(this.__image.fade, [delta].filter(undefinedFilter));
        });
        return this;
    }

    opacify () {
        this.__addOp(this.__image.opacify, []);
        return this;
    }

    hue () {
        judges.hue(arguments, shift => {
            this.__addOp(this.__image.hue, [shift].filter(undefinedFilter));
        });
        return this;
    }

    crop () {
        judges.crop(arguments, (left, top, right, bottom) => {
            this.__addOp(this.__image.crop, [left, top, right, bottom].filter(undefinedFilter));
        });
        return this;
    }

    mirror () {
        judges.mirror(arguments, axes => {
            this.__addOp(this.__image.mirror, [axes].filter(undefinedFilter));
        });
        return this;
    }

    flip () {
        return this.mirror.apply(this, arguments);
    }

    pad () {
        judges.pad(arguments, (left, top, right, bottom, color) => {
            color = normalizeColor(color);
            this.__addOp(this.__image.pad, [left, top, right, bottom, color].filter(undefinedFilter));
        });
        return this;
    }

    border () {
        judges.border(arguments, (width, color) => {
            color = normalizeColor(color);
            this.__addOp(this.__image.border, [width, color].filter(undefinedFilter));
        });
        return this;
    }

    sharpen () {
        judges.sharpen(arguments, amplitude => {
            this.__addOp(this.__image.sharpen, [amplitude].filter(undefinedFilter));
        });
        return this;
    }

    paste () {
        judges.paste(arguments, (left, top, img) => {
            this.__addOp(this.__image.paste, [left, top, img].filter(undefinedFilter));
        });
        return this;
    }

    setPixel () {
        judges.setPixel(arguments, (left, top, color) => {
            this.__addOp(this.__image.setPixel, [left, top, color].filter(undefinedFilter));
        });
        return this;
    }

    toBuffer () {
        judges.toBuffer(arguments, (type, params, callback) => {
            if (type === 'jpg' || type === 'jpeg') {
                util.normalizeJpegParams(params);
            } else if (type === 'png') {
                util.normalizePngParams(params);
            } else if (type === 'gif') {
                util.normalizeGifParams(params);
            } else throw Error('Unknown type \'' + type + '\'');
            this.exec((err, image) => {
                if (err) return callback(err);
                image.toBuffer(type, params, callback);
            });
        });
    }

    writeFile (outpath, type, params, callback) {
        judges.writeFile(arguments, (outpath, type, params, callback) => {
            type = type || path.extname(outpath).slice(1).toLowerCase();
            this.toBuffer(type, params, (err, buffer) => {
                if (err) return callback(err);
                fs.writeFile(outpath, buffer, {
                    encoding: 'binary'
                }, callback);
            });
        });
    }
}

module.exports = Batch;

// Extend Image with image.batch()
Image.prototype.batch = function () {
    return new Batch(this);
};

