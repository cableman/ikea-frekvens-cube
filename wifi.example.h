#include <ESP8266WiFi.h>

#ifndef __wifi_h
#define __wifi_h

struct WifiCreds {
    const char* ssid = "";
    const char* password = "";
};

#endif
