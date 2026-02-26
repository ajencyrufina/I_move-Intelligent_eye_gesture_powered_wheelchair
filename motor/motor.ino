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

// Ultrasonic Sensor
#define TRIG_PIN 34
#define ECHO_PIN 35

const int OBSTACLE_THRESHOLD = 10; // distance in cm
const int TURN_DELAY = 600; // milliseconds for 90° turn (adjust experimentally)

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

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

long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;  // cm
  return distance;
}

void turnRight90() {
  Serial.println("Turning Right 90°");

  digitalWrite(IN1, HIGH);  // left wheel forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // right wheel backward
  digitalWrite(IN4, HIGH);

  delay(TURN_DELAY); // adjust based on your robot

  stopMotors();
}

void loop() {
  long distance = readUltrasonicDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < OBSTACLE_THRESHOLD) {
    Serial.println("Obstacle Detected!");
    turnRight90();  // turn instead of stopping permanently
  }

  if (client.available()) {
    char received = client.read();
    Serial.print("Received: ");
    Serial.println(received);

    if (received == 'F') {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.println("Motor Forward");
    }
    else if (received == 'B') {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("Motor Backward");
    }
  }
}