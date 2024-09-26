const crypto = require('crypto');

var cipher_key = Buffer.from([0, 2, 4, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]);
var cipher_iv = Buffer.from([0, 6, 6, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]);

exports.aesEncrypt = function(text){
    const cipher = crypto.createCipheriv('aes-128-cbc',cipher_key,cipher_iv)
    text = new Buffer.from(text)
    var crypted = cipher.update(text,'utf-8','base64')
    crypted += cipher.final('base64');
    return crypted;
}

exports.aesDecrypt = function(text){
    const decipher = crypto.createDecipheriv('aes-128-cbc',cipher_key,cipher_iv)
    let dec = decipher.update(text,'base64','utf-8');
    dec += decipher.final();
    return dec;
}

