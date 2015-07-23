var imbase = 'images/',
    join = require('path').join;

module.exports = {
    inv: join(__dirname, imbase, 'no.such.image'),
    jpg: {
        gs: join(__dirname, imbase, 'gs.jpg'),
        rgb: join(__dirname, imbase, 'rgb.jpg'),
        noex: join(__dirname, imbase, 'rgbjpg'),
        inv: join(__dirname, imbase, 'invalid.jpg')
    },
    png: {
        gs: join(__dirname, imbase, 'gs.png'),
        rgb: join(__dirname, imbase, 'rgb.png'),
        noex: join(__dirname, imbase, 'rgbpng'),
        trans: join(__dirname, imbase, 'trans.png'),
        inv: join(__dirname, imbase, 'invalid.png'),
        hasMetadata: join(__dirname, imbase, 'hasMetadata.png'),
        noMetadata: join(__dirname, imbase, 'noMetadata.png')
    },
    gif: {
        gs: join(__dirname, imbase, 'gs.gif'),
        rgb: join(__dirname, imbase, 'rgb.gif'),
        noex: join(__dirname, imbase, 'rgbgif'),
        trans: join(__dirname, imbase, 'trans.gif'),
        inv: join(__dirname, imbase, 'invalid.gif')
    }
};
