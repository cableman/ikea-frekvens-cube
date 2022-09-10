#include "wifi.h"
#include <ezTime.h>

#include <Adafruit_GFX.h>
#include "Picopixel.h"

#include "FrekvensPanel.h"
#include "lemming_frames.h"

#define p_ena D5
#define p_data D0
#define p_clock D1
#define p_latch D2

#define p_btn1 D3 // YELLOW button (white wire)
#define p_btn2 D4 // RED button (black wire)

FrekvensPanel panel(p_latch, p_clock, p_data);
LemmingFrames lemming1;
LemmingFrames lemming2;

/**
 * Connect to wifi network.
 * 
 * Creds should be set in wifi.h.
 */
void connectWifi() 
{
  WifiCreds wifiCreds;
  
  WiFi.begin(wifiCreds.ssid, wifiCreds.password);

  bool state = true;
  while (WiFi.status() != WL_CONNECTED) {    
    panel.clear();
    panel.setCursor(0, 4);
    panel.println("WIFI!");
    if (state) {
      panel.println("WAIT");
    }
    panel.scan();
    state = !state;
    delay(400);
  }
}

/**
 * Connect to NTP time server.
 */
Timezone myTZ;
void connectTime() 
{
  panel.clear();
  panel.setCursor(0, 4);
  panel.print("Time");
  panel.scan();

  myTZ.setLocation("Europe/Copenhagen");
  waitForSync();
}

/**
 * Test if current program should be halted (program switch).
 */
bool breakProgram = false;
bool stopProgram() 
{
  if (breakProgram) {
    breakProgram = false;
    return true;
  }

  return false;
}

/**
 * The brightness can be controlled by button presses.
 * 
 * The is the halder for that.
 */
char activeBrightMode = 1;
void setBrightMode(int brightMode)
{
  activeBrightMode = brightMode % 4;
  switch(activeBrightMode)
  {
    case 0: { pinMode(p_ena, OUTPUT); digitalWrite(p_ena, 1); } break; // off
    case 1: analogWrite(p_ena, 200); break; // very dim
    case 2: analogWrite(p_ena, 100); break; // dim
    case 3: { pinMode(p_ena, OUTPUT); digitalWrite(p_ena, 0); } break; // on 100%
  }
}

/**
 * Set active program.
 */
char activeProgram = 3;
void setProgram(int program)
{
  activeProgram = (program % 4);
  breakProgram = true;
}

/**
 * Lemmings walking. 
 */
void program0()
{
  const int speed = 150;
  const int offsetRows = 3;

  for (int offsetCols = -6; offsetCols < 16+8; ++offsetCols) {
    panel.clear();

    if (stopProgram()) {
      break;
    }
    
    lemming1.nextFrame(panel, offsetRows, offsetCols);

    // refreshes display
    panel.scan();
  
    delay(speed);
  }
}

/**
 * Rotating triangle.
 */
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

/**
 * Random sparkels.
 */
void program2()
{
  for (int i=0;i<20;i++)
  {
    panel.drawPixel(random(16),random(16),random(10) == 0);
  }
  panel.scan();
  delay(5);
}

/**
 * Display time and working lemming.
 */
void program3()
{
  int offsetRows = 6;
  int speed = 150;
 
  for (int offsetCols = -6; offsetCols < 16+8; ++offsetCols) {
    panel.clear();

    if (stopProgram()) {
      break;
    }

    // Padd the hour with a space if the frist number is 1.
    panel.setCursor(0, 4);
    panel.print(myTZ.dateTime("H"));

    panel.setCursor(9, 4);
    panel.print(myTZ.dateTime("i"));
   
    lemming1.nextFrame(panel, offsetRows, offsetCols);

    // Refreshes display.
    panel.scan();
  
    delay(speed);
  }
}

/**
 * Yellow button interrupt handler.
 */
IRAM_ATTR void handleInterruptYellow() {
  setProgram(activeProgram+1);
}

/**
 * Red button interrupt handler.
 */
IRAM_ATTR void handleInterruptRed() {
  setBrightMode(activeBrightMode-1);
}

/**
 * Setup and get the show running.
 */
void setup() 
{
  // Clear panel and setup font.
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

/**
 * Main loop selecting active program.
 */
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
