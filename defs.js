(function() {

    var defaults = exports.defaults = {
        DEF_INTERPOLATION: 'lanczos',
        DEF_ROTATE_COLOR: 'gray',
        DEF_BORDER_COLOR: 'gray',
        DEF_PAD_COLOR: 'gray',
        DEF_JPEG_QUALITY: 100,
        PNG_DEF_COMPRESSION: 'fast',
        PNG_DEF_INTERLACED: false
    };

    exports.interpolations = {
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
            b: 0
        },
        'white': {
            r: 255,
            g: 255,
            b: 255
        },
        'red': {
            r: 255,
            g: 0,
            b: 0
        },
        'blue': {
            r: 0,
            g: 0,
            b: 255
        },
        'green': {
            r: 0,
            g: 255,
            b: 0
        },
        'cyan': {
            r: 0,
            g: 255,
            b: 255
        },
        'yellow': {
            r: 255,
            g: 255,
            b: 0
        },
        'gray': {
            r: 128,
            g: 128,
            b: 128
        },
        'magenta': {
            r: 255,
            g: 0,
            b: 255
        }
    };

    exports.args = {
        open: [{
            name: 'impath',
            type: 'string'
        }, {
            name: 'type',
            type: 'string',
            optional: true
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
            name: 'inter',
            type: 'string',
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
            name: 'inter',
            type: 'string',
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
            types: ['string', 'array', 'hash'],
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
            type: 'string'
        },{
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
        border: [{
            name: 'width',
            type: 'nn-number'
        }, {
            name: 'color',
            types: ['string', 'array', 'hash'],
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
            types: ['string', 'array', 'hash'],
            optional: true,
            default: defaults.DEF_PAD_COLOR
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
            optional: true,
        }, {
            name: 'params',
            type: 'hash',
            optional: true,
            default: {}
        }, {
            name: 'callback',
            type: 'function'
        }]
    }

})();
