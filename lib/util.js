(function(undefined) {

    var defs = require('./defs'),
        decree = require('decree'),
        Image = require('./Image');

    decree.register('color', validateColor);
    decree.register('interpolation', validateInterpolation);
    decree.register('axes', validateAxes);
    decree.register('image', validateImage);

    function undefinedFilter(v) {
        return v !== undefined;
    }

    function validateInterpolation(inter){
        return defs.interpolations.hasOwnProperty(inter) ||
            [1, 2, 3, 4, 5, 6].indexOf(inter) !== -1;
    }

    function validateAxes(axes){
        return ['x', 'y', 'xy', 'yx'].indexOf(axes) !== -1;
    }

    function validateImage(img){
        return img instanceof Image;
    }

    function validateColor(color) {
        if (typeof color === 'string') {
            if (!defs.colors[color]) return false;
        } else {
            if (color instanceof Array) {
                color = {
                    r: color[0],
                    g: color[1],
                    b: color[2],
                    a: color[3]
                };
            }
            var a = color.a;
            if (a !== 0) a = a || defs.defaults.DEF_COLOR_ALPHA; // (don't modify the original color object)
            if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255 ||
                color.g != parseInt(color.g) || color.g < 0 || color.g > 255 ||
                color.b != parseInt(color.b) || color.b < 0 || color.b > 255 ||
                a != parseInt(a) || a < 0 || a > 100)
                return false;
        }
        return true;
    }

    function normalizeColor(color) {
        if (typeof color === 'string') {
            color = defs.colors[color];
        } else if (color instanceof Array) {
            color = {
                r: color[0],
                g: color[1],
                b: color[2],
                a: color[3]
            };
        }
        if (color.a !== 0) color.a = color.a || defs.defaults.DEF_COLOR_ALPHA;
        return color;
    }

    function normalizePngParams(params){
        params.compression = params.compression || defs.defaults.PNG_DEF_COMPRESSION;
        if (params.compression === 'none') params.compression = 0;
        else if (params.compression === 'fast') params.compression = 1;
        else if (params.compression === 'high') params.compression = 2;
        else if ([0, 1, 2].indexOf(params.compression) === -1)
            throw Error('Invalid PNG compression');
        params.interlaced = params.interlaced || defs.defaults.PNG_DEF_INTERLACED;
        if (typeof params.interlaced !== 'boolean')
            throw Error('PNG \'interlaced\' must be boolean');
        params.transparency = params.transparency || defs.defaults.PNG_DEF_TRANSPARENT;
        if (typeof params.transparency !== 'boolean'){
            if (typeof params.transparency !== 'string' ||
                params.transparency.toLowerCase() !== 'auto')
                throw Error('PNG \'transparency\' must be boolean or \'auto\'');
        }
    }

    function normalizeJpegParams(params){
        if (params.quality !== 0)
            params.quality = params.quality || defs.defaults.DEF_JPEG_QUALITY;
        if (params.quality != parseInt(params.quality) ||
            params.quality < 0 || params.quality > 100)
            throw Error('Invalid JPEG quality');
    }

    module.exports = {
        undefinedFilter: undefinedFilter,
        normalizeColor: normalizeColor,
        normalizePngParams: normalizePngParams,
        normalizeJpegParams: normalizeJpegParams
    };

})(void 0);
