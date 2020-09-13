#include <SetUp.h>

#define WDT_TIMEOUT 40
/* create a hardware timer */
hw_timer_t *timer0 = NULL;  //Timer
hw_timer_t *timer1 = NULL;  //Timer
int RestartCounter = 0;     // counter to restart if restart button is presed
volatile byte state0 = LOW; // Timer interrupt flag
volatile byte state1 = LOW; // Timer interrupt flag
bool timer0Flag = 0;
bool timer1Flag = 0;

int Start; // Genaral counter used to count how many seconds have passed since a start

int x = 0;                   // General counter for loops ... etc
size_t i = 0;                // General counter for loops ... etc
bool SSIDUpdateDone;         // Flag
bool WifiPasswordUpdateDone; // Flag
char SSIDTemp[30];           // Temp String to store SSID data in before save it in the txt files
char PASSTemp[30];           // Temp String to store WIfi password data in before save it in the txt files
char VechileIDTemp[30];      // Temp String to store VechileID data in before save it in the txt files
char URLTemp[30];            // Temp String to store URL data in before save it in the txt files
int SensorTime = 5;          // Sensor change states after 60 seconds
int TimeInSeconds0 = 0;      // time counted by the timer is seconds
int TimeInSeconds1 = 0;      // time counted by the timer is seconds

char APSSID[16];
const char *APWifiPassword = "11223344";
// extern  Sensors = "000";
bool TimerFlag = 0;

void IRAM_ATTR onTimer0()
{

    state0 = !state0;
    TimeInSeconds0++;
}
void IRAM_ATTR onTimer1()
{
    state1 = !state1;
    TimeInSeconds1++;
}

void BoardInitialize(void)
{
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               //add current thread to WDT watch
    /* Use 1st timer of 4 */
    /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
    timer0 = timerBegin(0, 80, true);
    timer1 = timerBegin(1, 80, true);

    /* Attach onTimer function to our timer */
    timerAttachInterrupt(timer0, &onTimer0, true);
    timerAttachInterrupt(timer1, &onTimer1, true);

    /* Set alarm to call onTimer function every second 1 tick is 1us
  => 1 second is 1000000us */
    /* Repeat the alarm (third parameter) */
    timerAlarmWrite(timer0, 1000000, true);
    timerAlarmWrite(timer1, 1000000, true);

    pinMode(FrontSensor, INPUT);
    pinMode(RearSensorL, INPUT);
    pinMode(RearSensorR, INPUT);
}

void SetWifiAP()
{
    snprintf(APSSID, 16, "WWW%012llX", ESP.getEfuseMac());

    WiFi.softAP(APSSID, APWifiPassword);
    IPAddress IP = WiFi.softAPIP();
    // Serial.print("AP IP address: ");
    // Serial.println(IP);
}

void SetWifiClient()
{

    WiFi.mode(WIFI_STA);
    WiFi.setHostname("AL-JAWAB");
    WiFi.begin(&readFile(SPIFFS, "/SSID.txt")[0], &readFile(SPIFFS, "/WifiPassword.txt")[0]);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        // Serial.printf("WiFi Failed!\n");
    }
}

void DefaultSetting()
{

    while (touchRead(0) < 30)
    // while (!digitalRead (0))
    {
        // Serial.begin(115200);
        // Serial.println (touchRead (0));
        RestartCounter++;
        delay(100);
        if (RestartCounter > 50)
        { // 5 seconds
            SPIFFS.begin();
            writeFile(SPIFFS, "/SSID.txt", "Empty");
            writeFile(SPIFFS, "/WifiPassword.txt", "Empty");
            writeFile(SPIFFS, "/URL.txt", "http://192.168.1.94/Car/data.php?");
            writeFile(SPIFFS, "/VechileID.txt", "xxx1234");
            RestartCounter = 0;
            ESP.restart();
        }
   }
}

void SensorUpdate(SeatSensors &Sensors)
{
    if (Sensors[Front] != digitalRead(FrontSensor) + 48)
    {
        if (!timer0Flag)
        {
            timerAlarmEnable(timer0);
            timer0Flag = true;
        }
        if (TimeInSeconds0 >= SensorTime)
        {
            timerAlarmDisable(timer0);
            timer0Flag = false;
            TimeInSeconds0 = 0;
            Sensors[Front] = digitalRead(FrontSensor) + 48;
        }
    }

    if (Sensors[RearLeft] != digitalRead(RearSensorL) + 48)
    {
        if (!timer0Flag)
        {
            timerAlarmEnable(timer0);
            timer0Flag = true;
        }
        if (TimeInSeconds0 >= SensorTime)
        {
            timerAlarmDisable(timer0);
            timer0Flag = false;
            TimeInSeconds0 = 0;
            Sensors[RearLeft] = digitalRead(RearSensorL) + 48;
        }
    }

    if (Sensors[RearRight] != digitalRead(RearSensorR) + 48)
    {
        if (!timer0Flag)
        {
            timerAlarmEnable(timer0);
            timer0Flag = true;
        }
        if (TimeInSeconds0 >= SensorTime)
        {
            timerAlarmDisable(timer0);
            timer0Flag = false;
            TimeInSeconds0 = 0;
            Sensors[RearRight] = digitalRead(RearSensorR) + 48;
        }
    }
}

void UpdateSSIDandPassword(String Settings)
{
    Start = millis() / 1000; // the time when thiis function started to set a time out using millis() instead of timers
    // Serial.println("Start time is set");
    SSIDUpdateDone = false;
    x = 0;
    for (i = 0; i < Settings.length(); i++)
    {

        if (Settings[i] == '(' && !SSIDUpdateDone)
        {
            i += 1;
            while (Settings[i] != ')' && (millis() / 1000) - Start < 1)
            {
                SSIDTemp[x] = Settings[i];
                i += 1;
                x += 1;
            }
            x = 0;
            SSIDUpdateDone = true;
            writeFile2(SPIFFS, "/SSID.txt", SSIDTemp);
        }
        else if (Settings[i] == '(' && SSIDUpdateDone)
        {
            delay(10);
            i += 1;
            while (Settings[i] != ')' && (millis() / 1000) - Start < 3)
            {
                PASSTemp[x] = Settings[i];
                i += 1;
                x += 1;
            }
            x = 0;
            writeFile2(SPIFFS, "/WifiPassword.txt", PASSTemp);
        }
    }
    delay(1000);
    ESP.restart();
}

void UpdateVechileID(String &Settings)
{

    Start = millis() / 1000; // the time when thiis function started to set a time out using millis() instead of timers
    x = 0;
    for (i = 0; i < Settings.length(); i++)
    {
        if (Settings[i] == '(')
        {
            i += 1;
            while (Settings[i] != ')' && (millis() / 1000) - Start < 3)
            {
                VechileIDTemp[x] = Settings[i];
                i += 1;
                x += 1;
            }
            x = 0;
            writeFile2(SPIFFS, "/VechileID.txt", VechileIDTemp);
        }
    }
    delay(100);
    ESP.restart();
}

void UpdateURL(String &Settings)
{

    Start = millis() / 1000; // the time when thiis function started to set a time out using millis() instead of timers
    x = 0;
    for (i = 0; i < Settings.length(); i++)
    {
        if (Settings[i] == '(')
        {
            i += 1;
            while (Settings[i] != ')' && (millis() / 1000) - Start < 3)
            {
                VechileIDTemp[x] = Settings[i];
                i += 1;
                x += 1;
            }
            x = 0;
            writeFile2(SPIFFS, "/URL.txt", URLTemp);
        }
    }
    delay(100);
    ESP.restart();
}
