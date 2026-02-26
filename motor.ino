#include <WiFi.h>

const char* ssid = "narayanan";
const char* password = "xzjh1708";

const char* serverIP = "10.145.134.72";  
const uint16_t serverPort = 80;

WiFiClient client;

// Motor A
#define IN1 14
#define IN2 27

// Motor B
#define IN3 26
#define IN4 25

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.println("Connecting to Server...");

  while (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection Failed. Retrying...");
    delay(2000);
  }

  Serial.println("Connected to Server!");
}

void loop() {

  if (client.available()) {
    char received = client.read();

    Serial.print("Received: ");
    Serial.println(received);

    if (received == 'F') {
      // 🔵 FORWARD
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.println("Motor Forward");
    }

    else if (received == 'B') {
      // 🔴 BACKWARD
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("Motor Backward");
    }
  }
}