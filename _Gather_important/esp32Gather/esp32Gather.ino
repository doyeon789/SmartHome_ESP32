#include <Arduino_GFX_Library.h>
#include <HardwareSerial.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// OLED 디스플레이 크기 정의
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// TFT SPI 핀 정의
#define V_TFT_SCK    18
#define V_TFT_MOSI   23
#define V_TFT_MISO   19
#define V_TFT_CS     5
#define V_TFT_DC      2
#define V_TFT_RESET   4

// BLE 서비스 및 특성 UUID 정의
#define SERVICE_UUID        " /*유출주의*/"
#define CHARACTERISTIC_UUID " /*유출주의*/"

// 온습도 센서 초기화
DHT dht(16, DHT11);

// BLE 관련 전역 변수
BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;

// OLED 디스플레이 인스턴스 생성
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// TFT 디스플레이 인스턴스 생성
Arduino_ESP32SPI V_bus = Arduino_ESP32SPI(V_TFT_DC, V_TFT_CS, V_TFT_SCK, V_TFT_MOSI, V_TFT_MISO);
Arduino_ILI9488 V_display = Arduino_ILI9488(&V_bus, V_TFT_RESET);

// 비트맵 이미지 데이터 (Marilyn Monroe)
const unsigned char MarilynMonroe[] PROGMEM = { /* 생략 */ };

// BLE 기기 연결/해제 콜백 정의
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("✅ Device connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("❌ Device disconnected");
    delay(100);
    BLEDevice::startAdvertising(); // 연결 해제시 다시 광고 시작
    Serial.println("📢 Advertising restarted");
  }
};


void setup(void)
{
  Serial.begin(9600);  // 시리얼 통신 시작
  Wire.begin(21, 22);  // I2C 핀 설정

  // TFT 디스플레이 초기화
  V_display.begin();
  V_display.fillScreen(WHITE);
  V_display.setRotation(3);
  V_display.setTextColor(CYAN,WHITE);

  // OLED 디스플레이 초기화 및 비트맵 표시
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();
  display.drawBitmap(0, 0,  MarilynMonroe, 128, 64, WHITE);
  display.display();

  dht.begin(); // 온습도 센서 시작

  // BLE 초기화 및 설정
  BLEDevice::init("MyESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setValue("Hello World says ESP32!");
  pService->start();

  // BLE 광고 시작
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

// BLE 수신 데이터 처리용 문자열 및 인덱스
char str[40] = "00000000000\r\n";
char r_str[100] = "";
int idx = 0;

void loop() {
  static String parts[10]; // BLE 수신 데이터를 ',' 기준으로 나눠 저장

  // BLE에서 수신된 데이터 처리
  if (deviceConnected) {
    String value = pCharacteristic->getValue();

    if (value.length() > 0) {
      int index = 0;
      while (value.length() > 0) {
        int commaIndex = value.indexOf(',');
        if (commaIndex == -1) {
          parts[index++] = value;
          break;
        }
        parts[index++] = value.substring(0, commaIndex);
        value = value.substring(commaIndex + 1);
      }

      // BLE 수신값을 시리얼로 전송 → ATmega128에 전달
      Serial.write(parts[1].c_str()); // LED
      Serial.write(parts[3].c_str()); // RGB
      Serial.write(parts[6].c_str()); // Motor1
      Serial.write(parts[2].c_str()); // Motor2
      Serial.write('\n');

      pCharacteristic->setValue(""); // 수신 데이터 초기화
    }
  }

  // ATmega128 → ESP32 데이터 수신
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (idx > 0) {
        r_str[idx] = '\0';  // 문자열 종료
        idx = 0;
      }
    } else {
      if (idx < sizeof(r_str) - 1) {
        r_str[idx++] = c;  // 수신 문자 저장
      }
    }
  }

  // 온습도 측정
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // LCD에 스마트홈 상태 띄우기
  V_display.setCursor(0,10);
  V_display.setTextSize(3);
  V_display.print("1F");
  // ... (길어 생략)

  
  delay(500);
}
