var lib = require('bindings')('base58');
var crypto = require('crypto');

// Vanilla Base58 Encoding
var base58 = {
  encode: lib.base58_encode,
  decode: lib.base58_decode,
};
exports.base58 = base58;
exports.encode = base58.encode;
exports.decode = base58.decode;

// Base58Check Encoding
function sha256(data) {
  return new Buffer(crypto.createHash('sha256').update(data).digest('binary'), 'binary');
};

function doubleSHA256(data) {
  return sha256(sha256(data));
};

exports.base58Check = {
  encode: function(buf) {
    var checkedBuf = new Buffer(buf.length + 4);
    var hash = doubleSHA256(buf);
    buf.copy(checkedBuf);
    hash.copy(checkedBuf, buf.length);
    return base58.encode(checkedBuf);
  },

  decode: function(s) {
    var buf = base58.decode(s);
    if (buf.length < 4) {
      throw new Error("invalid input: too short");
    }

    var data = buf.slice(0, -4);
    var csum = buf.slice(-4);

    var hash = doubleSHA256(data);
    var hash4 = hash.slice(0, 4);

    if (csum.toString() != hash4.toString()) {
      throw new Error("checksum mismatch");
    }

    return data;
  },
};
