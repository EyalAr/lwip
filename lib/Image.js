(function(undefined) {

    var lwip_image = require('../build/Release/lwip_image');

    function Image(pixelsBuf, width, height, trans, metadata) {
        this.__lwip = new lwip_image.LwipImage(pixelsBuf, width, height);
        this.__locked = false;
        this.__trans = trans;
        this.__metadata = metadata;
    }

    // EXPORTS
    // -------
    module.exports = Image;

})(void 0);
