#include "wifi.h"

/**
 * Connect to wifi network.
 * 
 * Creds should be set in wifi_creds.h.
 */
void Wifi::connect(FrekvensPanel panel)
{
  bool connected = false;
  int active = 0;

  // Loop over creds until connection is established.
  while (false == connected) {
    connected = this->tryConnect(this->creds.ssid[active], this->creds.password[active], panel);
    active = ((active + 1) % WifiCredsNumber);
  }
}

/**
 * Try to connect to a network.
 */
bool Wifi::tryConnect(const char* ssid, const char* password, FrekvensPanel panel)
{
  WiFi.begin(ssid, password);

  bool state = true;
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {    
    panel.clear();
    panel.setCursor(0, 4);
    panel.println("WIFI!");
    if (state) {
      panel.println("WAIT");
    }
    panel.scan();
    delay(400);

    state = !state;
    attempts += 1;
  }
  
  return WiFi.status() == WL_CONNECTED;
}
