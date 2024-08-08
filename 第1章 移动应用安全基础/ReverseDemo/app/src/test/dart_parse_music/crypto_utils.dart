import 'dart:convert';
import 'package:encrypt/encrypt.dart';
import 'package:crypto/crypto.dart';

class CryptoUtils
{
  static final _key = Key.fromUtf8('jWJFtWSNhcAuDeEgMbzTdI9jBnSLuL0T');

  static final _iv = IV.fromUtf8('FDaNz4OyJkerWgII');

  static String aesEncrypt(String plaintext)
  {
    final encrypter = Encrypter(AES(_key));
    final encrypted = encrypter.encrypt(plaintext, iv: _iv);
    return encrypted.base64;
  }

  static String aesDecrypt(String encrypted)
  {
    final encrypter = Encrypter(AES(_key));
    final plaintext = encrypter.decrypt64(encrypted, iv: _iv);
    return plaintext;
  }

  static String SHA1(String plaintext)
  {
    var bytes = utf8.encode(plaintext); // data being hashed

    String digest = sha1.convert(bytes).toString();
    return digest.toUpperCase();
  }
  
  static String MD5(String plaintext)
  {
    var bytes = utf8.encode(plaintext); // data being hashed

    String digest = md5.convert(bytes).toString();
    return digest.toUpperCase();
  }

}


