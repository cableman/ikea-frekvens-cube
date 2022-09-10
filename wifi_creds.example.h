#ifndef __wifi_h
#define __wifi_h

#define WifiCredsNumber 4

struct WifiCreds {
    const char* ssid[WifiCredsNumber] = {
      "",
      "",
      "",
      "",
    };
    const char* password[WifiCredsNumber] = {
      "",
      "",
      "",
      "",
    };
};

#endif
