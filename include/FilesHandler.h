#ifndef FILESHANDLER_h_
#define FILESHANDLER_h_
  #include <SPIFFS.h>
  #include <FS.h>


String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void writeFile2(fs::FS &fs, const char *path, const String message);
#endif