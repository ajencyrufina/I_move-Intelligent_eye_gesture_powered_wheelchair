#include <WiFi.h>

#define SAMPLE_RATE 75
#define BAUD_RATE 115200
#define INPUT_PIN 36

// WiFi
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";
WiFiServer server(80);
WiFiClient client;

// Blink parameters
float threshold = 0.045;
unsigned long debounceTime = 200;
unsigned long minInterval = 700;
float eog;

bool prevAbove = false;
unsigned long blinkStartTime = 0;
unsigned long lastBlinkTime = 0;
bool motorState = false;  // false = Forward, true = Backward

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(INPUT_PIN, INPUT);
  delay(1000);

  // WiFi setup
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  // Handle client connection
  if (!client || !client.connected()) {
    client = server.available();
  }

  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  static long timer = 0;
  timer -= interval;

  if (timer < 0) {
    timer += 1000000 / SAMPLE_RATE;

    float raw_value = analogRead(INPUT_PIN);
    float filtered_signal = ECGFilter(raw_value) / 512.0;
    eog = filtered_signal / 8.0;

    detectBlink(eog);

    delay(1000);  // keeping as per your original code
  }
}

void detectBlink(float eog) {

  bool currentAbove = (abs(eog) > threshold);
  unsigned long currentTime = millis();

  // Rising edge
  if (currentAbove && !prevAbove) {
    blinkStartTime = currentTime;
  }

  // Falling edge
  if (!currentAbove && prevAbove) {

    unsigned long blinkDuration = currentTime - blinkStartTime;

    if (blinkDuration > debounceTime &&
        currentTime - lastBlinkTime > minInterval) {

      motorState = !motorState;

      if (!motorState) {
        Serial.println("F");
        Serial.println("Forward Triggered");

        if (client && client.connected()) {
          client.println("F");
        }

      } else {
        Serial.println("B");
        Serial.println("Backward Triggered");

        if (client && client.connected()) {
          client.println("B");
        }
      }

      lastBlinkTime = currentTime;
    }
  }

  prevAbove = currentAbove;
}

// FILTER (UNCHANGED)
float ECGFilter(float input) {
  float output = input;

  {
    static float z1, z2;
    float x = output - 0.02977423 * z1 - 0.04296318 * z2;
    output = 0.09797471 * x + 0.19594942 * z1 + 0.09797471 * z2;
    z2 = z1;
    z1 = x;
  }

  {
    static float z1, z2;
    float x = output - 0.08303952 * z1 - 0.46067709 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }

  {
    static float z1, z2;
    float x = output - 1.92167271 * z1 - 0.92347975 * z2;
    output = 1.00000000 * x - 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }

  {
    static float z1, z2;
    float x = output - 1.96758991 * z1 - 0.96933514 * z2;
    output = 1.00000000 * x - 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }

  return output;
}