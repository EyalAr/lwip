'use strict';

const path = require('path'),
    fs = require('fs'),
    decree = require('decree'),
    defs = require('./defs'),
    util = require('./util'),
    encoder = require('bindings')('lwip_encoder'),
    lwip_image = require('bindings')('lwip_image'),
    normalizeColor = util.normalizeColor;

const judges = {
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

module.exports = class Image {
    constructor (pixelsBuf, width, height, trans, metadata) {
        this.__lwip = new lwip_image.LwipImage(pixelsBuf, width, height);
        this.__locked = false;
        this.__trans = trans;
        this.__metadata = metadata;
    }

    __lock () {
        if (!this.__locked) this.__locked = true;
        else throw Error("Another image operation already in progress");
    }

    __release () {
        this.__locked = false;
    }

    getMetadata () {
        return this.__metadata;
    }

    setMetadata (data) {
        if (typeof data != "string" && data != null) {
            throw Error("Metadata must be a string or null");
        }

        this.__metadata = data;
        return data;
    }

    width () {
        return this.__lwip.width();
    }

    height () {
        return this.__lwip.height();
    }

    size () {
        return {
            width: this.__lwip.width(),
            height: this.__lwip.height()
        }
    }

    getPixel () {
        const args = judges.getPixel(arguments),
            left = args[0],
            top = args[1];

        if (left >= this.width() || top >= this.height())
            throw Error("Coordinates exceed dimensions of image");

        const rgba = this.__lwip.getPixel(left, top);

        return {
            r: rgba[0],
            g: rgba[1],
            b: rgba[2],
            a: rgba[3]
        };
    }

    scale () {
        this.__lock();
        judges.scale(
            arguments,
            (wRatio, hRatio, inter, callback) => {
                hRatio = hRatio || wRatio;
                const width = +wRatio * this.width(),
                    height = +hRatio * this.height();
                this.__lwip.resize(width, height, defs.interpolations[inter], err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    resize () {
        this.__lock();
        judges.resize(
            arguments,
            (width, height, inter, callback) => {
                height = height || width;
                this.__lwip.resize(+width, +height, defs.interpolations[inter], err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    contain () {
        judges.contain(
            arguments,
            (width, height, color, inter, callback) => {
                const s = Math.min(width / this.width(), height / this.height());
                this.scale(s, s, inter, err => {
                    if (err) return callback(err);
                    const padX = (width - this.width()) / 2,
                        padY = (height - this.height()) / 2;
                    this.pad(
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
    }

    cover () {
        judges.cover(
            arguments,
            (width, height, inter, callback) => {
                const s = Math.max(width / this.width(), height / this.height());
                this.scale(s, s, inter, err => {
                    if (err) return callback(err);
                    this.crop(width, height, callback);
                });
            }
        );
    }

    rotate () {
        this.__lock();
        judges.rotate(
            arguments,
            (degs, color, callback) => {
                color = normalizeColor(color);
                if (color.a < 100) this.__trans = true;
                this.__lwip.rotate(+degs, +color.r, +color.g, +color.b, +color.a, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    blur () {
        this.__lock();
        judges.blur(
            arguments,
            (sigma, callback) => {
                this.__lwip.blur(+sigma, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    hslaAdjust () {
        this.__lock();
        judges.hslaAdjust(
            arguments,
            (hs, sd, ld, ad, callback) => {
                this.__lwip.hslaAdj(+hs, +sd, +ld, +ad, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    saturate () {
        this.__lock();
        judges.saturate(
            arguments,
            (delta, callback) => {
                this.__lwip.hslaAdj(0, +delta, 0, 0, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    lighten () {
        this.__lock();
        judges.lighten(
            arguments,
            (delta, callback) => {
                this.__lwip.hslaAdj(0, 0, +delta, 0, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    darken () {
        this.__lock();
        judges.darken(
            arguments,
            (delta, callback) => {
                this.__lwip.hslaAdj(0, 0, -delta, 0, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    fade () {
        this.__lock();
        judges.fade(
            arguments,
            (delta, callback) => {
                this.__lwip.hslaAdj(0, 0, 0, -delta, err => {
                    if (+delta > 0) this.__trans = true;
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    opacify () {
        this.__lock();
        judges.opacify(
            arguments,
            callback => {
                this.__lwip.opacify(err => {
                    this.__trans = false;
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    hue () {
        this.__lock();
        judges.hue(
            arguments,
            (shift, callback) => {
                this.__lwip.hslaAdj(+shift, 0, 0, 0, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    crop () {
        this.__lock();
        judges.crop(
            arguments,
            (left, top, right, bottom, callback) => {
                if (!right && !bottom) {
                    const size = this.size(),
                        width = left,
                        height = top;
                    left = 0 | (size.width - width) / 2;
                    top = 0 | (size.height - height) / 2;
                    right = left + width - 1;
                    bottom = top + height - 1;
                }
                this.__lwip.crop(left, top, right, bottom, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    mirror () {
        this.__lock();
        judges.mirror(
            arguments,
            (axes, callback) => {
                let xaxis = false,
                    yaxis = false;
                if ('x' === axes) xaxis = true;
                if ('y' === axes) yaxis = true;
                if ('xy' === axes || 'yx' === axes) {
                    xaxis = true;
                    yaxis = true;
                }
                this.__lwip.mirror(xaxis, yaxis, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    flip () {
        return this.mirror.apply(this, arguments);
    }

    pad () {
        this.__lock();
        judges.pad(
            arguments,
            (left, top, right, bottom, color, callback) => {
                color = normalizeColor(color);
                if (color.a < 100) this.__trans = true;
                this.__lwip.pad(+left, +top, +right, +bottom, +color.r, +color.g, +color.b, +color.a, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    border () {
        this.__lock();
        judges.border(
            arguments,
            (width, color, callback) => {
                color = normalizeColor(color);
                if (color.a < 100) this.__trans = true;
                // we can just use image.pad...
                this.__lwip.pad(+width, +width, +width, +width, +color.r, +color.g, +color.b, +color.a, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    sharpen () {
        this.__lock();
        judges.sharpen(
            arguments,
            (amplitude, callback) => {
                this.__lwip.sharpen(+amplitude, err => {
                    this.__release();
                    callback(err, this);
                });
            },
            err => {
                this.__release();
                throw err;
            }
        );
    }

    paste () {
        this.__lock();
        try{
            judges.paste(
                arguments,
                (left, top, img, callback) => {
                    // first we retrieve what we need (buffer, dimensions, ...)
                    // synchronously so that the pasted image doesn't have a chance
                    // to be changed
                    const pixbuff = img.__lwip.buffer(),
                        width = img.__lwip.width(),
                        height = img.__lwip.height();
                    if (left + width > this.__lwip.width() || top + height > this.__lwip.height())
                        throw Error("Pasted image exceeds dimensions of base image");
                    this.__lwip.paste(+left, +top, pixbuff, +width, +height, err => {
                        this.__release();
                        callback(err, this);
                    });
                }
            );
        } catch(err){
            this.__release();
            throw err;
        }
    }

    setPixel () {
        this.__lock();
        try{
            judges.setPixel(
                arguments,
                (left, top, color, callback) => {
                    if (left >= this.width() || top >= this.height())
                        throw Error("Coordinates exceed dimensions of image");
                    color = normalizeColor(color);
                    if (color.a < 100) this.__trans = true;
                    this.__lwip.setPixel(+left, +top, +color.r, +color.g, +color.b, +color.a, err => {
                        this.__release();
                        callback(err, this);
                    });
                }
            );
        } catch(err){
            this.__release();
            throw err;
        }
    }

    clone () {
        // no need to lock the image. we don't modify the memory buffer.
        // just copy it.
        judges.clone(
            arguments,
            callback => {
                // first we retrieve what we need (buffer, dimensions, ...)
                // synchronously so that the original image doesn't have a chance
                // to be changed (remember, we don't lock it); and only then call
                // the callback asynchronously.
                const pixbuff = this.__lwip.buffer(),
                    width = this.__lwip.width(),
                    height = this.__lwip.height(),
                    trans = this.__trans;
                setImmediate(() => {
                    callback(null, new Image(pixbuff, width, height, trans));
                });
            }
        );
    }

    extract () {
        // no need to lock the image. we don't modify the memory buffer.
        // just copy it and then crop it.
        judges.extract(
            arguments,
            (left, top, right, bottom, callback) => {
                // first we retrieve what we need (buffer, dimensions, ...)
                // synchronously so that the original image doesn't have a chance
                // to be changed (remember, we don't lock it); then we crop it and
                // only call the callback asynchronously.
                const pixbuff = this.__lwip.buffer(),
                    width = this.__lwip.width(),
                    height = this.__lwip.height(),
                    trans = this.__trans,
                    eximg = new Image(pixbuff, width, height, trans);
                eximg.__lwip.crop(left, top, right, bottom, err => {
                    callback(err, eximg);
                });
            }
        );
    }

    toBuffer () {
        this.__lock();
        try{
            judges.toBuffer(
                arguments,
                (type, params, callback) => {
                    const encoderCb = (err, buffer) => {
                        this.__release();
                        callback(err, buffer);
                    }
                    if (type === 'jpg' || type === 'jpeg') {
                        util.normalizeJpegParams(params);
                        return encoder.jpeg(
                            this.__lwip.buffer(),
                            this.__lwip.width(),
                            this.__lwip.height(),
                            params.quality,
                            encoderCb
                        );
                    } else if (type === 'png') {
                        util.normalizePngParams(params);
                        return encoder.png(
                            this.__lwip.buffer(),
                            this.__lwip.width(),
                            this.__lwip.height(),
                            params.compression,
                            params.interlaced,
                            params.transparency === 'auto' ? this.__trans : params.transparency,
                            this.__metadata,
                            encoderCb
                        );
                    } else if (type === 'gif') {
                        util.normalizeGifParams(params);
                        return encoder.gif(
                            this.__lwip.buffer(),
                            this.__lwip.width(),
                            this.__lwip.height(),
                            util.getClosest2Exp(params.colors),
                            params.colors,
                            params.interlaced,
                            params.transparency === 'auto' ? this.__trans : params.transparency,
                            params.threshold,
                            encoderCb
                        );
                    } else throw Error('Unknown type \'' + type + '\'');
                }
            );
        } catch (err){
            this.__release();
            throw err;
        }
    }

    writeFile () {
        judges.writeFile(
            arguments,
            (outpath, type, params, callback) => {
                type = type || path.extname(outpath).slice(1).toLowerCase();
                this.toBuffer(type, params, (err, buffer) => {
                    if (err) return callback(err);
                    fs.writeFile(outpath, buffer, {
                        encoding: 'binary'
                    }, callback);
                });
            }
        );
    }
}
