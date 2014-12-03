var lwip = require('./'), i;
lwip.open('examples/lena.jpg',function(err, image){
    if (err) return console.log(err);
    image.writeFile('examples/lena_e.gif',{colors:5, interlaced: true, transparency: false},function(err){
        if (err) console.log(err);
    });
});
