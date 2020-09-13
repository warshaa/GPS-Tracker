#ifndef WebServer1_H
#define WebServer1_H

#ifdef ESP8266
#include <Updater.h>
#include <ESP8266mDNS.h>
#else
#include <Update.h>
#include <ESPmDNS.h>
#endif
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>
  #include <FS.h>




void setupWebServer(void);
void setupUpdate(void);
void writeFile(fs::FS &fs, const char * path, const char * message);
String readFile(fs::FS &fs, const char * path);

#endif