#include <ESP8266WiFi.h>

#include "FrekvensPanel.h"
#include "wifi_creds.h"

#ifndef __wifi_h
#define __wifi_h

#define maxAttempts 10

class Wifi
{
  private:
    WifiCreds creds;
    bool tryConnect(const char* ssid, const char* password, FrekvensPanel panel);
  
  public:
    void connect(FrekvensPanel pane);
};

#endif
