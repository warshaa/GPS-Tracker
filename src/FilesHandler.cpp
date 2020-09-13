#include <FilesHandler.h>

String readFile(fs::FS &fs, const char *path)
{
    SPIFFS.begin(true);
    // Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        // Serial.println("- empty file or failed to open file");
        file.close();
        SPIFFS.end();
        return String();
    }
    // Serial.println("- read from file:");
    String fileContent;
    while (file.available())
    {
        fileContent += String((char)file.read());
    }
    //Serial.println(fileContent);
    file.close();
    SPIFFS.end();
    return fileContent;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    SPIFFS.begin(true);
    //Serial.printf("Writing file: %s\r\n", path);
    File file = fs.open(path, "w");
    if (!file)
    {
        //Serial.println("- failed to open file for writing");
        file.close();
        return;
    }
    if (file.print(message))
    {
        //Serial.println("- file written");
    }
    else
    {
        //Serial.println("- write failed");
    }
    file.close();
    SPIFFS.end();
}

void writeFile2(fs::FS &fs, const char *path,  String message)
{
    SPIFFS.begin(true);
    //Serial.printf("Writing file: %s\r\n", path);
    File file = fs.open(path, "w");
    if (!file)
    {
        //Serial.println("- failed to open file for writing");
        file.close();
        return;
    }
    if (file.print(message))
    {
        //Serial.println("- file written");
    }
    else
    {
        //Serial.println("- write failed");
    }
    file.close();
    SPIFFS.end();
}
