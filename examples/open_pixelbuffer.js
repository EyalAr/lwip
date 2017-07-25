/**
 * Example for using LWIP to open a raw pixel buffer
 */

const path = require('path'),
    lwip = require('../');

const w = 90;
const h = 90;
const channelSize = w * h;
const size = channelSize * 4;
const redChannelEnd = channelSize * 1;
const greenChannelEnd = channelSize * 2;
const blueChannelEnd = channelSize * 3;
const alphaChannelEnd = channelSize * 4;
let i, x;

const buffer = new Buffer(size);
for(i = blueChannelEnd; i < alphaChannelEnd; i++) {
	buffer[i] = 100;
}
for (let y = 0; y < h; y++) {
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

lwip.open(buffer, { width: w, height: h }, (err, image) => {
    if (err) return console.log("err open", err);
    image.batch()
    	.blur(9)
    	.writeFile('image_from_pixelbuffer.png', err => {
    		if (err) return console.log("err write", err);
        console.log('done');
    	});
});
