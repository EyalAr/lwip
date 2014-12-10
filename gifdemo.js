var lwip = require('./'), i;
lwip.open('tests/images/trans.png',function(err, image){
    if (err) return console.log(err);
    image.writeFile('trans.gif',{colors:256, interlaced: false, transparency: true},function(err){
        if (err) console.log(err);
    });
});
