(function(undefined) {

    function Batch(image) {
        this.__image = image;
        this.__queue = [];
        this.__running = false;
    }

    Batch.prototype.__addOp = function(handle, args) {
        this.__queue.push({
            handle: handle,
            args: args
        });
    };

    // EXPORTS
    // -------
    module.exports = Batch;

})(void 0);
