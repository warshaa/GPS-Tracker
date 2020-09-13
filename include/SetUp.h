#ifndef SETUP_H_
#define SETUP_H_

#include <WString.h>
#include <esp32-hal-timer.h>
#include <WiFi.h>
#include <FilesHandler.h>
#include <stdio.h>
#include <esp_task_wdt.h>


//Seat Sensors
#define FrontSensor 27
#define RearSensorL 26
#define RearSensorR 25

extern int TimeInSeconds1;
extern bool timer1Flag;
extern hw_timer_t *timer1;
extern String VechileId;

typedef String SeatSensors;

enum
{
    _TimeInterval = 48,
    _OnSensorChange = 49,
    _WhileSensorsOn = 50,
    _UpdateSSIDandPassword = 51,
    _UpdateVechileID = 52,
    _UpdateURL = 53,
    _SetDeviceAsServer = 54,
    _Restart = 120

};

enum
{
    _GPSOn,
    _GPSOff
};

enum
{
    Front,
    RearLeft,
    RearRight
};


void
BoardInitialize(void);
void SetWifiAP(void);                        // set the device as Wi-Fi AP (Access point) with default SSID and PAssword
void SetWifiClient(void);                    // set the Device as Wi-Fi Client after the User inputs SSID and Password
void DefaultSetting(void);                   //Reset the device set it as Wi-Fi AP(Acces Point) (Default)
void UpdateSSIDandPassword(String Settings); // update the Wifi SSID and Password from the server
String GetGps(void);                         // return gps cordinates
void SensorUpdate(SeatSensors &Sensors);     //update Sensors states
void UpdateVechileID(String &Settings);      // update vechile ID
void UpdateURL(String &Settings);            // update vechile ID

#endif