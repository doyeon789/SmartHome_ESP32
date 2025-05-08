// 320x240
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


/* 480x320
#include <Arduino_GFX_Library.h>

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     15
#define TFT_DC      2
#define TFT_RESET   4

Arduino_ESP32SPI bus = Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_ILI9488 display = Arduino_ILI9488(&bus, TFT_RESET);  // 드라이버 수정!

void setup(void)
{
  display.begin();
  display.setRotation(2);  // 가로 방향: 480x320

  display.fillScreen(WHITE);
  display.setCursor(40, 20);
  display.setTextSize(2);
  display.setTextColor(BLUE);
  display.print("From Arduino");

  display.setCursor(25, 50);
  display.print("This is the text");

  display.setCursor(70, 80);
  display.print("statement");

  display.fillRect(150, 200, 100, 100, RED);  // 여유 있게 위치 조정
}

void loop() {}

*/
