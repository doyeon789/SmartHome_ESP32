#include <Arduino_GFX_Library.h>

#define CS    15
#define RESET 4
#define DC    2
#define SDI   23
#define SCK   18

void setup() {
  Arduino_DataBus *bus = new Arduino_HWSPI(DC , CS);
  Arduino_GFX *display = new Arduino_ILI9341(bus,RESET);

  display->begin();
  display->setRotation(3);
  display->fillScreen(RGB565_BLACK);
  display->setCursor(10, 10);
  display->setTextColor(RGB565_WHITE);
  display->setTextSize(5);
  display->println("Hello\nWorld!");
}

void loop() {
  // put your main code here, to run repeatedly:

} 
