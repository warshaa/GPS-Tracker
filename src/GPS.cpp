#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "GPS.h"
#include <sstream>
TinyGPSPlus gps;
String Location = "x.xxxxxxxxxx,y.yyyyyyyyyyy";
char Time[6];
char Date[8];
String Speed="0.000000";
int Temp;
String TimeStampx = "0000000000000";
int StartTime;

String GetLocation()
{
    StartTime = millis();
    while (Serial2.available() > 0 || (millis() - StartTime < 1500))
    // while (Serial2.available() > 0 )
    {
        if (gps.encode(Serial2.read()) && gps.location.isValid() )
        {
            return (DoubleToString(gps.location.lat()) + "," + DoubleToString(gps.location.lng()));
        }
    }
    return "00";
}

String GetTimeStamp()
{
    StartTime = millis();
    while (Serial2.available() > 0 || (millis() - StartTime < 1500))
    // while (Serial2.available() > 0 )
        if (gps.encode(Serial2.read()) && gps.date.isValid() )
        {
            Temp = gps.date.day() + (gps.date.month() * 100) + (gps.date.year() * 10000);
            sprintf(Date, "%d", Temp);
            for (size_t i = 0; i < 8; i++)
            {
                TimeStampx[i] = Date[i];
            }
        }
    if (gps.time.isValid())
    {
        Temp = (gps.time.second()) + (gps.time.minute() * 100) + (gps.time.hour() * 10000);
        sprintf(Time, "%d", Temp);
        for (size_t i = 8; i < 14; i++)
        {
            TimeStampx[i] = Time[i - 8];
        }
    }
    return TimeStampx;
}

String GetSpeed()
{
    // StartTime = millis();
        while (Serial2.available() > 0 || (millis() - StartTime < 1500))
    while (Serial2.available() > 0)
        if (gps.encode(Serial2.read()) && gps.speed.isValid() )
            return (DoubleToString(gps.speed.kmph()));
    return "00";
}

String DoubleToString(double x)
{
    char c[20];
    sprintf(c, "%f", x);
    return c;
}
