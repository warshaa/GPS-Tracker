#include "WebServer1.h"
#include "SendToServer.h"
#include "GPS.h"
#include "SetUp.h"

String GetLink = "";
String URL = "";
const String SensorsVariable = "sensors=";
const String GPSVariable = "gps=";
const String SpeedVariable = "speed=";
const String VechileIdVariable = "vechile_id=";
const String TimeStampVariable = "time_stamp=";
const String OnTimeVariable = "on_time=";
String GPSCordinates="x.xxxxxxxxx,y.yyyyyyyyyyyy";
String speed="0.000000";
String Settings = "00.10000000000000000000000000";
SeatSensors Sensors = "000";
String VechileId = "xxx123456789";
String TimeStamp="190012121212121";
bool OnSensorChangeFlag = 0; // if the sensor changed its status the flag becomes true
int TimeInterval = 2;

char Interval = 0;
bool ServerFlag = false;
int TimeCounter = 0;
int WifiFailCounter=0;

void setup()
{
    // Serial.begin(115200);
    Serial2.begin(9600);
    BoardInitialize();
    URL = readFile(SPIFFS, "/URL.txt");
    VechileId = readFile(SPIFFS, "/VechileID.txt");

    if (readFile(SPIFFS, "/SSID.txt") == "Empty")
    {
        SetWifiAP();
        setupWebServer();
        ServerFlag = true;
    }
    else
    {
        SetWifiClient();
        setupUpdate();
    }
}

void loop()
{

    /*
    --------------------------------Data Structure From Server---------------------------------------------
    Server will reply String of 4 chars that will be stored in the String Variable 'Settings'.
    Settings[0] the first element sets when the device should send get request to the server as follow:
    case '0' Send get request to the server every X of time .
    Case '1' Send get request to the server Upon the change of the any of sensors status.
    case '2' Send get request to the server while any sensor is On with Time Interval.
    case '3' Update Wifi SSID and PaSerial2word Settings [4]=( => SSID Name between first two brackets, paSerial2 word between second two brackets. (ie. Settings = "3123(SSIDNAME)(PASerial2WORD)")
    case '4' Update Vechile ID    
    case '5' Update URL         
    case '6' Set the Device As Server      

    case '9' Restart the Device

    Settings [1] The second element is to Control the GPS as follow:
    case '0'  turn of the GPS.
    case '1'  turn on the GPS and send the GPS cordinates in every get request.

    Settings [2,3] X time or time interval in minutes if Settings [2] == '.' then Settings [3] = time in seconds.
    --------------------------------End Of Data Structure From Server----------------------------------------
    */
    while (ServerFlag != true)
    {
        DefaultSetting(); // if the reset button is hold for 5 seconds the device restart into default settings
        esp_task_wdt_reset();

        if (WiFi.status() != WL_CONNECTED)
        {
            WifiFailCounter++;
            SetWifiClient();
            if (WiFi.status() != WL_CONNECTED && WifiFailCounter > 9)
                ESP.restart();
        }
        else
        {

            // delay(1000);
            GPSCordinates = GetLocation();
            speed = GetSpeed();
            // TimeStamp = GetTimeStamp();
            SensorUpdate(Sensors);

            GetLink = URL + SensorsVariable + Sensors + "&" + GPSVariable +  GPSCordinates  + "&" + SpeedVariable + speed + "&" + OnTimeVariable + (millis() / 1000) + "&" + VechileIdVariable + VechileId ;
            //+ "&" + TimeStampVariable + TimeStamp;jtn

            switch (Settings[0])
            {
            case _TimeInterval:
                if (!timer1Flag)
                {
                    timerAlarmEnable(timer1);
                    timer1Flag = true;
                }

                if (TimeInterval < 2) // Minimum Time Interval is 2 seconds
                    TimeInterval = 2;
                if (TimeInSeconds1 >= TimeInterval)
                {
                    timerAlarmDisable(timer1);
                    timer1Flag = false;
                    TimeInSeconds1 = 0;
                    Settings = SendToServer(GetLink);

                    // first validate data the two are real numbers
                    if (Settings[2] >= 48 && Settings[2] <= 57 && Settings[3] >= 48 && Settings[3] <= 57)
                    {
                        TimeInterval = ((Settings[3] - 48) + ((Settings[2] - 48) * 10)) * 60; // time from server in minutes to seconds
                    }
                    // validata data if there is a point (ie 0.6 = 60 seconds)
                    else if (Settings[2] == 46 && Settings[3] >= 48 && Settings[3] <= 57)
                    {
                        TimeInterval = ((Settings[3] - 48) * 10); // time from server in minutes to seconds
                    }
                }
                break;

            case _OnSensorChange:

                if (OnSensorChangeFlag)
                {
                    Settings = SendToServer(GetLink);
                    OnSensorChangeFlag = false;
                }

                break;

            case _WhileSensorsOn:

                if (Sensors[Front] == '1' || Sensors[RearLeft] == '1' || Sensors[RearRight] == '1')
                {
                    if (!timer1Flag)
                    {
                        timerAlarmEnable(timer1);
                        timer1Flag = true;
                    }
                    if (TimeInterval < 2) // Minimum Time Interval is 2 seconds
                        TimeInterval = 2;
                    if (TimeInSeconds1 >= TimeInterval)
                    {
                        timerAlarmDisable(timer1);
                        timer1Flag = false;
                        TimeInSeconds1 = 0;
                        Settings = SendToServer(GetLink);
                    }
                }
                break;

            case _UpdateSSIDandPassword:
                UpdateSSIDandPassword(Settings);
                break;

            case _UpdateVechileID:
                UpdateVechileID(Settings);
                break;

            case _UpdateURL:
                UpdateURL(Settings);
                break;

                // case _SetDeviceASerial2erver:
                //     UpdateSSIDandPaSerial2word(Settings);
                //     break;

            case _Restart:
                ESP.restart();
                break;

            default:
                delay(5000);
                Settings = SendToServer(GetLink);
                break;
            }
        }
    }
    DefaultSetting();
    esp_task_wdt_reset();
}
