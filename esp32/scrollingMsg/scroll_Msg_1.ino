#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4   // Number of 8x8 matrix modules
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  myDisplay.begin();
  myDisplay.setIntensity(5); // Brightness 0-15
  myDisplay.displayClear();
  myDisplay.displayScroll("Hello ESP32! Welcome World", PA_CENTER, PA_SCROLL_LEFT, 100);
}

void loop() {
  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
}

