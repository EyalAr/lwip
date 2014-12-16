var lwip = require('./'), i;
lwip.open('tests/images/trans.png',function(err, image){
    if (err) return console.log(err);
    image.writeFile('s_f_p.gif',{colors:255, interlaced: false, transparency: true, threshold: 60},function(err){
        if (err) console.log(err);
    });
});
