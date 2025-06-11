#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;

// 👉 콜백 클래스 정의
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("✅ Device connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("❌ Device disconnected");

    delay(100);  // 잠깐 기다린 뒤
    BLEDevice::startAdvertising();  // 💡 다시 광고 시작
    Serial.println("📢 Advertising restarted");
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 Starting BLE work!");

  // BLE 초기화
  BLEDevice::init("MyESP32");

  // 서버, 서비스, 특성 생성
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());  // 콜백 등록

  BLEService* pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setValue("Hello World says ESP32!");
  pService->start();

  // 광고 설정 및 시작
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // iOS compatibility
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("✅ BLE setup complete! You can now connect in your phone app.");
}


void loop() {
  if (deviceConnected) {
    String value = pCharacteristic->getValue();

    if (value.length() > 0) {
      Serial.println(value);

      pCharacteristic->setValue("");  // 데이터 초기화
    }
  }

  delay(100);
}
