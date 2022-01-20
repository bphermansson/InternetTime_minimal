// Minimal internet time fetcher for ESP32 based devices.
#include <WiFi.h>
#include "InternetTime_minimal.h"

const char* ssid     = "BrandstorpWifi";                    // your network SSID (name)
const char* password = "Brandstorp";                    // your network password
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.persistent(false);  // Do not write Wifi settings to flash
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint8_t connAttempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(". %i \n", connAttempts);
    connAttempts++;
    if (connAttempts>15) {
      Serial.println(F("WiFi connection error, check your settings."));    
      Serial.println((String)"In settings ssid="+ssid+" and pass="+password);      
      while(connAttempts > 15) {
        yield();
      }
    }
    delay(1000);
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  printLocalTime();
  delay(1000);
}

void printLocalTime()
{
  struct tm time;
   
  if(!getLocalTime(&time)){
    Serial.println("Could not obtain time info");
    return;
  }
  char my_time[20];
  snprintf(my_time, 17, "%d-%02d-%02d %02d:%02d\n", (time.tm_year+1900), time.tm_mon+1,time.tm_mday, time.tm_hour, time.tm_min);
  Serial.println(my_time);
}

