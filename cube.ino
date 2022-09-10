#include "wifi.h"
#include <ezTime.h>

#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>

#include "FrekvensPanel.h"
#include "lemming_frames.h"

#define p_ena D5
#define p_data D0
#define p_clock D1
#define p_latch D2

#define p_btn1 D3 // YELLOW button (white wire)
#define p_btn2 D4 // RED button (black wire)

Timezone myTZ;

FrekvensPanel panel(p_latch, p_clock, p_data);

LemmingFrames lemming1;

void connectWifi() {
  WifiCreds wifiCreds;
  
  bool state = true;
  WiFi.begin(wifiCreds.ssid, wifiCreds.password);
  while (WiFi.status() != WL_CONNECTED) {    
    panel.clear();
    panel.setCursor(0, 4);
    panel.println("WIFI!");
    if (state) {
      panel.println("...");
      state = !state;
    }
    panel.scan();

    delay(400);
  }
}

void connectTime() {
  panel.clear();
  panel.setCursor(0, 4);
  panel.print("Time");
  panel.scan();

  myTZ.setLocation("Europe/Copenhagen");
  waitForSync();
}

char activeBrightMode = 1;
void setBrightMode(int brightMode)
{
  activeBrightMode = brightMode % 4;
  switch(activeBrightMode)
  {
    case 0: { pinMode(p_ena, OUTPUT); digitalWrite(p_ena, 1); } break; // off
    case 1: analogWrite(p_ena, 240); break; // very dim
    case 2: analogWrite(p_ena, 150); break; // dim
    case 3: { pinMode(p_ena, OUTPUT); digitalWrite(p_ena, 0); } break; // on 100%
  }
}

char activeProgram = 3;
void setProgram(int program)
{
  activeProgram = (program % 4);
}

void program0()
{
  const int speed = 150;
  const int offsetRows = 3;

  for (int offsetCols = -6; offsetCols < 16+8; ++offsetCols) {
    panel.clear();
    
    lemming1.nextFrame(panel, offsetRows, offsetCols);

    // refreshes display
    panel.scan();
  
    delay(speed);
  }
}

int p1_i = 0;
void program1()
{
  float pi23 = PI*2/3;
  float p = 0.02 * p1_i++;
  int x0 = 7.98 + 7.98 * cos(p);
  int y0 = 7.98 + 7.98 * sin(p);
  p += pi23;
  int x1 = 7.98 + 7.98 * cos(p);
  int y1 = 7.98 + 7.98 * sin(p);
  p += pi23;
  int x2 = 7.98 + 7.98 * cos(p);
  int y2 = 7.98 + 7.98 * sin(p);
  panel.clear();
  panel.drawLine(x0,y0,x1,y1,1);  
  panel.drawLine(x1,y1,x2,y2,1);  
  panel.drawLine(x2,y2,x0,y0,1);

  panel.scan();
}

void program2()
{
  for (int i=0;i<20;i++)
  {
    panel.drawPixel(random(16),random(16),random(10) == 0);
  }
  panel.scan();
}

bool state = true;
void program3()
{
  int offsetRows = 6;
  int speed = 150;
  
  for (int offsetCols = -6; offsetCols < 16+8; ++offsetCols) {
    panel.clear();

    panel.setCursor(0, 4);
    panel.print(myTZ.dateTime("H"));  
  
    panel.setCursor(9, 4);
    panel.print(myTZ.dateTime("i"));
   
    lemming1.nextFrame(panel, offsetRows, offsetCols);

    // refreshes display
    panel.scan();
  
    delay(speed);
  }

   
   panel.scan();
}

IRAM_ATTR void handleInterruptYellow() {
  setProgram(activeProgram+1);
}

IRAM_ATTR void handleInterruptRed() {
  setBrightMode(activeBrightMode-1);
}

void setup() 
{
  panel.clear();
  panel.scan();
  panel.setFont(&Picopixel);
  panel.setTextSize(1);

  // Brightness - 100%
  pinMode(p_ena, OUTPUT); 
  analogWrite(p_ena, 0); 

  // Button setup.
  pinMode(p_btn1, INPUT); 
  pinMode(p_btn2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt( p_btn1 ), handleInterruptYellow, FALLING ); 
  attachInterrupt(digitalPinToInterrupt( p_btn2 ), handleInterruptRed, FALLING );

  // Connect to WIFI.
  connectWifi();

  // Connect to time servers.
  connectTime();
}

void loop() 
{
  switch(activeProgram)
  {
    case 0: program0(); break;
    case 1: program1(); break;
    case 2: program2(); break;
    case 3: program3(); break;
  }
}
