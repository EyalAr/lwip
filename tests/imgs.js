var imbase = 'images/',
    join = require('path').join;

module.exports = {
    inv: join(__dirname, imbase, 'no.such.image'),
    jpg: {
        gs: join(__dirname, imbase, 'gs.jpg'),
        rgb: join(__dirname, imbase, 'rgb.jpg'),
        noex: join(__dirname, imbase, 'rgbjpg'),
        inv: join(__dirname, imbase, 'invalid.jpg')
    }
};
