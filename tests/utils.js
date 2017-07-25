module.exports = {
    generateRandomBatch: generateRandomBatch,
    getRandomColor: getRandomColor
};

function generateRandomBatch(batch, n) {
    const ops = [];
    for (let i = 0; i < n; i++) {
        const r = Math.floor(Math.random() * 16);
        let w, h, d;
        switch (r) {
            case 0:
                const sd = Math.floor(Math.random() * 20);
                batch = batch.blur(sd);
                ops.push('blr' + sd);
                break;
            case 1:
                const a = Math.floor(Math.random() * 360);
                batch = batch.rotate(a, getRandomColor());
                ops.push('rtt' + a);
                break;
            case 2:
                w = Math.floor(Math.random() * 1000) + 10;
                h = Math.floor(Math.random() * 1000) + 10;
                batch = batch.resize(w, h);
                ops.push('rsz' + w + 'X' + h);
                break;
            case 3:
                w = Math.floor(Math.random() * 1000) + 10;
                h = Math.floor(Math.random() * 1000) + 10;
                batch = batch.crop(w, h);
                ops.push('crp' + w + 'X' + h);
                break;
            case 4:
                const wr = Math.random() * 1 + 0.1;
                const hr = Math.random() * 1 + 0.1;
                batch = batch.scale(wr, hr);
                ops.push('scl' + wr.toFixed(2) + 'X' + hr.toFixed(2));
                break;
            case 5:
                const i = Math.floor(Math.random() * 3);
                const axes = ['x', 'y', 'xy'][i];
                batch = batch.mirror(axes);
                ops.push('mrr' + axes);
                break;
            case 6:
                const b = Math.floor(Math.random() * 30);
                batch = batch.border(b, getRandomColor());
                ops.push('brdr' + b);
                break;
            case 7:
                const amp = Math.floor(Math.random() * 300);
                batch = batch.sharpen(amp);
                ops.push('shrp' + amp);
                break;
            case 8:
                d = Math.random() * 2;
                batch = batch.saturate(d);
                ops.push('sat' + d.toFixed(2));
                break;
            case 9:
                d = Math.random() * 2;
                batch = batch.lighten(d);
                ops.push('lit' + d.toFixed(2));
                break;
            case 10:
                d = Math.random() * 0.5;
                batch = batch.darken(d);
                ops.push('drk' + d.toFixed(2));
                break;
            case 11:
                const s = Math.floor(Math.random() * 360);
                batch = batch.hue(s);
                ops.push('hue' + s);
                break;
            case 12:
                d = Math.random() * 2 - 1;
                batch = batch.fade(d);
                ops.push('fad' + d.toFixed(2));
                break;
            case 13:
                batch = batch.opacify();
                ops.push('opc');
                break;
            case 14:
                w = Math.floor(Math.random() * 1000) + 10;
                h = Math.floor(Math.random() * 1000) + 10;
                batch = batch.contain(w, h, getRandomColor());
                ops.push('cnt' + w + 'X' + h);
                break;
            case 15:
                w = Math.floor(Math.random() * 1000) + 10;
                h = Math.floor(Math.random() * 1000) + 10;
                batch = batch.cover(w, h);
                ops.push('cvr' + w + 'X' + h);
                break;
        }
    }
    return ops;
}

function getRandomColor() {
    return {
        r: Math.floor(Math.random() * 256),
        g: Math.floor(Math.random() * 256),
        b: Math.floor(Math.random() * 256),
        a: Math.floor(Math.random() * 50) + 51
    };
}
