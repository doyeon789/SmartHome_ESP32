#include <Arduino_GFX_Library.h>

#define V_TFT_SCK    18
#define V_TFT_MOSI   23
#define V_TFT_MISO   19
#define V_TFT_CS     5
#define V_TFT_DC     2
#define V_TFT_RESET  4

#define H_TFT_SCK    14
#define H_TFT_MOSI   13
#define H_TFT_MISO   12
#define H_TFT_CS     15
#define H_TFT_DC     25
#define H_TFT_RESET  26


Arduino_ESP32SPI V_bus = Arduino_ESP32SPI(V_TFT_DC, V_TFT_CS, V_TFT_SCK, V_TFT_MOSI, V_TFT_MISO);
Arduino_ILI9488 V_display = Arduino_ILI9488(&V_bus, V_TFT_RESET);

Arduino_ESP32SPI H_bus = Arduino_ESP32SPI(H_TFT_DC, H_TFT_CS, H_TFT_SCK, H_TFT_MOSI, H_TFT_MISO);
Arduino_ILI9341 H_display = Arduino_ILI9341(&H_bus, H_TFT_RESET);

void setup(void)
{
  V_display.begin();
  V_display.setRotation(3);

  V_display.fillScreen(WHITE);
  V_display.setCursor(40, 20);
  V_display.setTextSize(2);
  V_display.setTextColor(BLUE);
  V_display.print("V_SPI");

  V_display.setCursor(25, 50);
  V_display.print("This is the text");

  V_display.setCursor(70, 80);
  V_display.print("statement");

  V_display.fillRect(150, 200, 100, 100, RED);  // 여유 있게 위치 조정

  // MAKR: --
  
  H_display.begin();
  H_display.setRotation(3);

  H_display.fillScreen(WHITE);
  H_display.setCursor(40, 20);
  H_display.setTextSize(2);
  H_display.setTextColor(BLUE);
  H_display.print("H_SPI");

  H_display.setCursor(25, 50);
  H_display.print("This is the text");

  H_display.setCursor(70, 80);
  H_display.print("statement");

  H_display.fillRect(150, 100, 100, 100, PURPLE);  // 여유 있게 위치 조정
}

void loop() {}
