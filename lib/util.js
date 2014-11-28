(function(undefined) {

    var defs = require('./defs'),
        decree = require('decree');

    decree.register('color', validateColor);

    function undefinedFilter(v) {
        return v !== undefined;
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

    module.exports = {
        undefinedFilter: undefinedFilter,
        normalizeColor: normalizeColor
    };

})(void 0);
