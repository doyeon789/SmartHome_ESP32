#include <Arduino_GFX_Library.h>
#include <HardwareSerial.h>

#define V_TFT_SCK    18
#define V_TFT_MOSI   23
#define V_TFT_MISO   19
#define V_TFT_CS     5
#define V_TFT_DC      2
#define V_TFT_RESET   4

Arduino_ESP32SPI V_bus = Arduino_ESP32SPI(V_TFT_DC, V_TFT_CS, V_TFT_SCK, V_TFT_MOSI, V_TFT_MISO);
Arduino_ILI9488 V_display = Arduino_ILI9488(&V_bus, V_TFT_RESET);

void setup(void)
{
  Serial.begin(9600); //기존의 기본 시리얼

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
}

char str[40] = "11111111111002\r\n";

char r_str[100] = "";  // 수신 버퍼
int idx = 0;           // 현재 위치

void loop() {
  Serial.write(str);
  delay(1000);

  // 수신
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (idx > 0) {
        r_str[idx] = '\0';  // 문자열 끝 표시
        Serial.println(r_str);
        idx = 0;            // 버퍼 초기화
      }
    } else {
      if (idx < sizeof(r_str) - 1) {
        r_str[idx++] = c;   // 버퍼에 저장
      }
    }
  }
}
