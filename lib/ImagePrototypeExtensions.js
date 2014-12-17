(function(undefined) {

    var Image = require('./Image'),
        decree = require('decree'),
        defs = require('./defs');

    var judges = {
        contain: decree(defs.args.contain)
    };

    Image.prototype.contain = function() {
        var that = this;
        judges.contain(
            arguments,
            function(width, height, color, inter, callback) {
                var s = Math.min(width / that.width(), height / that.height());
                that.scale(s, s, inter, function(err){
                    if (err) return callback(err);
                    var padX = (width - that.width()) / 2,
                        padY = (height - that.height()) / 2;
                    that.pad(
                        Math.ceil(padX),
                        Math.ceil(padY),
                        Math.floor(padX),
                        Math.floor(padY),
                        color,
                        callback
                    );
                });
            }
        );
    };

})(void 0);
