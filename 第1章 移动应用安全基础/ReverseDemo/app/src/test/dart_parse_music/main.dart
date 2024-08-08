import 'dart:io';
import 'crypto_utils.dart';

void scanFiles(String path)
{
  try{
      var  directory = new Directory(path);
      List<FileSystemEntity> files = directory.listSync();
      for(var tmp in files)
      {
        print(tmp.path);
      }
  }
  catch(e)
  {
    print(e);
  }
}



void main() 
{
  print(CryptoUtils.aesEncrypt("hello world"));
  File mFile =  new File('result');
  mFile.writeAsString(CryptoUtils.SHA1("wwwwwwrrrrr"));
  scanFiles('/Users/tal/Projects/dart/parse_music');
}
