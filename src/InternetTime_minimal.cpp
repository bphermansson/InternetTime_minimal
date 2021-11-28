// Minimal internet time fetcher for ESP8266 based devices.
#if defined(ESP8266)
  #pragma message "ESP8266"
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #pragma message "ESP32"
  #include <WiFi.h>
#else
  #error "This ain't a ESP8266 or ESP32, dumbo!"
#endif

#include "InternetTime_minimal.h"

const char* ssid     = "BrandstorpWifi";                    // your network SSID (name)
const char* password = "Brandstorp";                    // your network password
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
char buffer[80];

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("\nWaiting for time");
  
  while (!time(nullptr))
  {
    Serial.print(".");
    delay(1000);
  }
  delay(1000);
  Serial.println("Setup done.");

}

void loop() {
  printLocalTime();
  delay(1000);
}

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80," %d %B %Y %H:%M:%S ",timeinfo);
  Serial.println(buffer);

#if defined(ESP32)
  // %S doesnt work on the ESP32, so we use 'raw time' as epoch. 
  Serial.println(rawtime);
#endif
}
