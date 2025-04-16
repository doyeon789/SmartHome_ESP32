#include <HardwareSerial.h>

void setup() {
    Serial.begin(9600); //기존의 기본 시리얼
}

void loop() {
    Serial.write("hello\r\n");
    delay(1000);
}