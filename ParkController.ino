#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>


const char *ssid = "NetSpeed 2.4GHz";
const char *password = "a2b539ac";

AsyncWebServer server(80);

const int ledPin = 16; 
const int servoPin = 18;
Servo myServo;
int ledState = 0;

void setup() {
  Serial.begin(115200);

  // WiFi bağlantısı
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(".");
  // Statik IP ayarları
  IPAddress newIP(192, 168, 1, 201);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(newIP, gateway, subnet);
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // REST API endpoint'i tanımlama
  server.on("/slot/control", HTTP_POST, [](AsyncWebServerRequest *request) {
    String ledValue = request->arg("input");
    Serial.println("Received arg: " + ledValue);
    ledState = ledValue.toInt();

    // LED kontrolü
    if (ledState == 1) {
      digitalWrite(ledPin, HIGH); // LED'i aç
      Serial.println("Lojik 1");
    } else if (ledState == 0) {
      digitalWrite(ledPin, LOW); // LED'i kapat
      Serial.println("lojik 0");
    } else {
      Serial.println("lojik mojik yok :)");
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid input state\"}");
      return;
    }

    request->send(200, "application/json", "{\"status\":\"success\"}");
  });

  // LED pinini çıkış olarak ayarla
  pinMode(ledPin, OUTPUT);
  // Servo motorunu başlatma
  myServo.attach(servoPin);
  myServo.write(0);
  // Sunucuyu başlatma
  server.begin();
}

void loop() {
    if (ledState == 1) {
    myServo.write(90); // Servo motoru 90 dereceye döndür
  } else if (ledState == 0) {
    myServo.write(0); // Servo motoru geri eski haline al
  }
}
