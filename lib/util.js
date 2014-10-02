(function(undefined) {

    var defs = require('./defs');

    function undefinedFilter(v) {
        return v !== undefined;
    }

    function normalizeColor(color) {
        if (typeof color === 'string') {
            if (defs.colors[color]) color = defs.colors[color];
            else throw Error('Unknown color ' + color);
        } else {
            if (color instanceof Array) {
                color = {
                    r: color[0],
                    g: color[1],
                    b: color[2],
                    a: color[3]
                };
            }
            if (color.a !== 0) color.a = color.a || defs.defaults.DEF_COLOR_ALPHA;
            if (color.r != parseInt(color.r) || color.r < 0 || color.r > 255)
                throw Error('\'red\' color component is invalid');
            if (color.g != parseInt(color.g) || color.g < 0 || color.g > 255)
                throw Error('\'green\' color component is invalid');
            if (color.b != parseInt(color.b) || color.b < 0 || color.b > 255)
                throw Error('\'blue\' color component is invalid');
            if (color.a != parseInt(color.a) || color.a < 0 || color.a > 100)
                throw Error('\'alpha\' color component is invalid');
        }
        return color;
    }

    module.exports = {
        undefinedFilter: undefinedFilter,
        normalizeColor: normalizeColor
    };

})(void 0);
