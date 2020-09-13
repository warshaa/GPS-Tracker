#include <SendToServer.h>
#include <HttpClient.h>
#include <SetUp.h>

String SendToServer(String URL) {

    HTTPClient http;
    http.begin(URL); //HTTP
    int httpCode = http.GET();
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled

        // Serial.println("[HTTP] GET... code: %d\n");
        // Serial.println(httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            http.end();
            return payload;
            // Serial.println(payload);
        }
        else
        {
            http.end();
            return "NO Response From Server";
            // Serial.println("[HTTP] GET... failed, error: %s\n");
            // Serial.println(http.errorToString(httpCode).c_str());
        }
    }
    else
    {
        return "NO Response From Server";
    }
    
}