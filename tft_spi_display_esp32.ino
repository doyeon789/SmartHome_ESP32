#include <Arduino_GFX_Library.h>

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
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
  display.setRotation(2);
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
