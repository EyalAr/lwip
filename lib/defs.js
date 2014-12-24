(function() {

    var defaults = exports.defaults = {
        DEF_INTERPOLATION: 'lanczos',
        DEF_ROTATE_COLOR: 'gray',
        DEF_BORDER_COLOR: 'gray',
        DEF_PAD_COLOR: 'gray',
        DEF_COLOR_ALPHA: 100,
        DEF_JPEG_QUALITY: 100,
        PNG_DEF_COMPRESSION: 'fast',
        PNG_DEF_INTERLACED: false,
        PNG_DEF_TRANSPARENT: 'auto',
        GIF_DEF_COLORS: 256,
        GIF_DEF_INTERLACED: false,
        GIF_DEF_TRANSPARENT: 'auto',
        GIF_DEF_THRESHOLD: 50,
        DEF_CREATE_COLOR: [0, 0, 0, 0],
        DEF_CONTAIN_COLOR: [0, 0, 0, 0]
    };

    var interpolations = exports.interpolations = {
        'nearest-neighbor': 1,
        'moving-average': 2,
        'linear': 3,
        'grid': 4,
        'cubic': 5,
        'lanczos': 6
    };

    exports.colors = {
        'black': {
            r: 0,
            g: 0,
            b: 0,
            a: 100
        },
        'white': {
            r: 255,
            g: 255,
            b: 255,
            a: 100
        },
        'red': {
            r: 255,
            g: 0,
            b: 0,
            a: 100
        },
        'blue': {
            r: 0,
            g: 0,
            b: 255,
            a: 100
        },
        'green': {
            r: 0,
            g: 255,
            b: 0,
            a: 100
        },
        'cyan': {
            r: 0,
            g: 255,
            b: 255,
            a: 100
        },
        'yellow': {
            r: 255,
            g: 255,
            b: 0,
            a: 100
        },
        'gray': {
            r: 128,
            g: 128,
            b: 128,
            a: 100
        },
        'magenta': {
            r: 255,
            g: 0,
            b: 255,
            a: 100
        }
    };

    exports.args = {
        open: [{
            name: 'source',
            type: '*'
        }, {
            name: 'type',
            types: ['string', 'raw-buffer-properties'],
            optional: true
        }, {
            name: 'callback',
            type: 'function'
        }],
        create: [{
            name: 'width',
            type: 'p-number'
        }, {
            name: 'height',
            type: 'p-number'
        }, {
            name: 'color',
            type: 'color',
            optional: true,
            default: defaults.DEF_CREATE_COLOR
        }, {
            name: 'callback',
            type: 'function'
        }],
        scale: [{
            name: 'wRation',
            type: 'p-number'
        }, {
            name: 'hRatio',
            type: 'p-number',
            optional: true
        }, {
            name: 'interpolation',
            type: 'interpolation',
            optional: true,
            default: defaults.DEF_INTERPOLATION
        }, {
            name: 'callback',
            type: 'function'
        }],
        resize: [{
            name: 'width',
            type: 'p-number'
        }, {
            name: 'height',
            type: 'p-number',
            optional: true
        }, {
            name: 'interpolation',
            type: 'interpolation',
            optional: true,
            default: defaults.DEF_INTERPOLATION
        }, {
            name: 'callback',
            type: 'function'
        }],
        rotate: [{
            name: 'degs',
            type: 'number'
        }, {
            name: 'color',
            type: 'color',
            optional: true,
            default: defaults.DEF_ROTATE_COLOR
        }, {
            name: 'callback',
            type: 'function'
        }],
        blur: [{
            name: 'sigma',
            type: 'nn-number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        hslaAdjust: [{
            name: 'hs',
            type: 'number'
        }, {
            name: 'sd',
            type: 'number'
        }, {
            name: 'ld',
            type: 'number'
        }, {
            name: 'ad',
            type: 'number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        saturate: [{
            name: 'delta',
            type: 'number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        lighten: [{
            name: 'delta',
            type: 'number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        darken: [{
            name: 'delta',
            type: 'number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        fade: [{
            name: 'delta',
            type: 'number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        opacify: [{
            name: 'callback',
            type: 'function'
        }],
        hue: [{
            name: 'shift',
            type: 'number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        crop: [{
            name: 'left',
            type: 'nn-number'
        }, {
            name: 'top',
            type: 'nn-number'
        }, {
            name: 'right',
            type: 'nn-number',
            optional: true
        }, {
            name: 'bottom',
            type: 'nn-number',
            optional: true
        }, {
            name: 'callback',
            type: 'function'
        }],
        mirror: [{
            name: 'axes',
            type: 'axes'
        }, {
            name: 'callback',
            type: 'function'
        }],
        exec: [{
            name: 'callback',
            type: 'function'
        }],
        toBuffer: [{
            name: 'type',
            type: 'string'
        }, {
            name: 'params',
            type: 'hash',
            optional: true,
            default: {}
        }, {
            name: 'callback',
            type: 'function'
        }],
        getPixel: [{
            name: 'left',
            type: 'nn-int'
        }, {
            name: 'top',
            type: 'nn-int'
        }],
        border: [{
            name: 'width',
            type: 'nn-number'
        }, {
            name: 'color',
            type: 'color',
            optional: true,
            default: defaults.DEF_BORDER_COLOR
        }, {
            name: 'callback',
            type: 'function'
        }],
        pad: [{
            name: 'left',
            type: 'nn-number'
        }, {
            name: 'top',
            type: 'nn-number'
        }, {
            name: 'right',
            type: 'nn-number'
        }, {
            name: 'bottom',
            type: 'nn-number'
        }, {
            name: 'color',
            type: 'color',
            optional: true,
            default: defaults.DEF_PAD_COLOR
        }, {
            name: 'callback',
            type: 'function'
        }],
        sharpen: [{
            name: 'amplitude',
            type: 'number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        clone: [{
            name: 'callback',
            type: 'function'
        }],
        paste: [{
            name: 'left',
            type: 'nn-number'
        }, {
            name: 'top',
            type: 'nn-number'
        }, {
            name: 'image',
            type: 'image'
        }, {
            name: 'callback',
            type: 'function'
        }],
        extract: [{
            name: 'left',
            type: 'nn-number'
        }, {
            name: 'top',
            type: 'nn-number'
        }, {
            name: 'right',
            type: 'nn-number'
        }, {
            name: 'bottom',
            type: 'nn-number'
        }, {
            name: 'callback',
            type: 'function'
        }],
        writeFile: [{
            name: 'path',
            type: 'string'
        }, {
            name: 'type',
            type: 'string',
            optional: true
        }, {
            name: 'params',
            type: 'hash',
            optional: true,
            default: {}
        }, {
            name: 'callback',
            type: 'function'
        }],
        setPixel: [{
            name: 'left',
            type: 'nn-int'
        }, {
            name: 'top',
            type: 'nn-int'
        }, {
            name: 'color',
            type: 'color'
        }, {
            name: 'callback',
            type: 'function'
        }],
        contain: [{
            name: 'width',
            type: 'p-number'
        }, {
            name: 'height',
            type: 'p-number'
        }, {
            name: 'color',
            type: 'color',
            optional: true,
            default: defaults.DEF_CONTAIN_COLOR
        }, {
            name: 'interpolation',
            type: 'interpolation',
            optional: true,
            default: defaults.DEF_INTERPOLATION
        }, {
            name: 'callback',
            type: 'function'
        }],
        cover: [{
            name: 'width',
            type: 'p-number'
        }, {
            name: 'height',
            type: 'p-number'
        }, {
            name: 'interpolation',
            type: 'interpolation',
            optional: true,
            default: defaults.DEF_INTERPOLATION
        }, {
            name: 'callback',
            type: 'function'
        }],
    };

})();
