/**
 * Example for using LWIP to open a raw pixel buffer
 */

var path = require('path'),
    lwip = require('../');

var w = 90;
var h = 90;
var channelSize = w * h;
var size = channelSize * 4;
var redChannelEnd = channelSize * 1;
var greenChannelEnd = channelSize * 2;
var blueChannelEnd = channelSize * 3;
var alphaChannelEnd = channelSize * 4;
var i, x;

var buffer = new Buffer(size);
for(i = blueChannelEnd; i < alphaChannelEnd; i++) {
	buffer[i] = 100;
}
for(var y = 0; y < h; y++) {
	for(x = 0; x < 30; x++) {
		i = y * w + x;
		buffer[i] = 255;
		buffer[i + redChannelEnd] = 0;
		buffer[i + greenChannelEnd] = 0;
	}
	for(x = 30; x < 60; x++) {
		i = y * w + x;
		buffer[i] = 0;
		buffer[i + redChannelEnd] = 255;
		buffer[i + greenChannelEnd] = 0;
	}
	for(x = 60; x < 90; x++) {
		i = y * w + x;
		buffer[i] = 0;
		buffer[i + redChannelEnd] = 0;
		buffer[i + greenChannelEnd] = 255;
	}
}

lwip.open(buffer, { width: w, height: h }, function(err, image) {
    if (err) return console.log("err open", err);
    image.batch()
    	.blur(9)
    	.writeFile('image_from_pixelbuffer.png', function(err){
    		if (err) return console.log("err write", err);
        console.log('done');
    	});
});
