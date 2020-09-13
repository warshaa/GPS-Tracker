#include "WebServer1.h"
#include <WebPages.h>

AsyncWebServer server(80);
AsyncWebServer server1(80);

size_t content_len;
char* host = "UPDATE";

String SSID1; //wifi SSID
String PASS1; //wifi password
String outputState(int output);

/*
parrameters from website to update variables
*/

const char *SSID = "SSID";
const char *WifiPassword = "PASS";
const char *VechileID = "ID";
const char *URLx = "URLx";
const char *Restart = "restart";
const char *UpdateFlag = "UpdateFlag";

String processor(const String &var)
{
  if (var == "M1")
  {
    return readFile(SPIFFS, "/SSID.txt");
  }
  else if (var == "M2")
  {
    return readFile(SPIFFS, "/WifiPassword.txt");
  }
  else if (var == "M3")
  {
    return readFile(SPIFFS, "/VechileID.txt");
  }
  else if (var == "M4")
  {
    return readFile(SPIFFS, "/URL.txt");
  }
  return String();
}

const char *PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void handleUpdate(AsyncWebServerRequest *request)
{
  char *html = "<form method='POST' action='/doUpdate' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
  request->send(200, "text/html", html);
}

void handleDoUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    //Serial.println("Update");
    content_len = request->contentLength();
    // if filename includes spiffs, update the spiffs partition
    int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
#ifdef ESP8266
    Update.runAsync(true);
    if (!Update.begin(content_len, cmd))
    {
#else
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
    {
#endif
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len)
  {
    Update.printError(Serial);
#ifdef ESP8266
  }
  else
  {
    //Serial.printf("Progress: %d%%\n", (Update.progress() * 100) / Update.size());
#endif
  }

  if (final)
  {
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
    response->addHeader("Refresh", "20");
    response->addHeader("Location", "/");
    request->send(response);
    if (!Update.end(true))
    {
      Update.printError(Serial);
    }
    else
    {
      //Serial.println("Update complete");
      Serial.flush();
      ESP.restart();
    }
  }
}

void printProgress(size_t prg, size_t sz)
{
  //Serial.printf("Progress: %d%%\n", (prg * 100) / content_len);
}

boolean webInit()
{
  server1.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->redirect("/update"); });
  server1.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) { handleUpdate(request); });
  server1.on(
      "/doUpdate", HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
         size_t len, bool final) { handleDoUpdate(request, filename, index, data, len, final); });
  server1.onNotFound([](AsyncWebServerRequest *request) { request->send(404); });
  server1.begin();
#ifdef ESP32
  Update.onProgress(printProgress);
#endif
}

void setupUpdate()
{
  // snprintf(host, 16, "ESP%012llX", ESP.getEfuseMac());
  MDNS.begin(host);
  if (webInit())
    MDNS.addService("http", "tcp", 80);
  //Serial.printf("Ready! Open http://%s.local in your browser\n", host);
}

void setupWebServer()
{

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", Index);
  });

  server.on("/JQXXMNWER", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", JQXXMNWER, processor);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", Updatex);
  });
  /////
  // char host[16];
  // snprintf(host, 16, "ESP%012llX", ESP.getEfuseMac());
  // MDNS.begin(host);
  // if(webInit()) MDNS.addService("http", "tcp", 80);
  // //Serial.printf("Ready! Open http://%s.local in your browser\n", host);
  ////

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(SSID))
    {
      inputMessage = request->getParam(SSID)->value();
      writeFile(SPIFFS, "/SSID.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(WifiPassword))
    {
      inputMessage = request->getParam(WifiPassword)->value();
      writeFile(SPIFFS, "/WifiPassword.txt", inputMessage.c_str());
    }
    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    else if (request->hasParam(VechileID))
    {
      inputMessage = request->getParam(VechileID)->value();
      writeFile(SPIFFS, "/VechileID.txt", inputMessage.c_str());
    }
    else if (request->hasParam(URLx))
    {
      inputMessage = request->getParam(URLx)->value();
      writeFile(SPIFFS, "/URL.txt", inputMessage.c_str());
    }
    else if (request->hasParam(Restart))
    {
      ESP.restart();
    }
    else if (request->hasParam(UpdateFlag))
    {
      delay(200);
      server.end();
      delay(200);
      setupUpdate();
    }
    else
    {
      inputMessage = "No message sent";
    }
  });

  server.onNotFound(notFound);

  server.begin();
}