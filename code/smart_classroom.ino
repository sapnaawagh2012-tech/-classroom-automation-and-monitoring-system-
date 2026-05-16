#include <DHT.h>

// DHT setup
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pins
#define PIR_PIN 13
#define FAN_RELAY 5
#define LIGHT_RELAY 4

// Variables
unsigned long lastMotionTime = 0;
const int delayTime = 10000; // 10 sec

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(FAN_RELAY, OUTPUT);
  pinMode(LIGHT_RELAY, OUTPUT);

  dht.begin();

  // OFF initially (active LOW relay)
  digitalWrite(FAN_RELAY, HIGH);
  digitalWrite(LIGHT_RELAY, HIGH);
}

void loop() {
  float temp = dht.readTemperature();
  int motion = digitalRead(PIR_PIN);

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" | Motion: ");
  Serial.println(motion);

  // FAN LOGIC (Temp + Motion BOTH required)
  if (temp >= 30 && motion == HIGH) {
    digitalWrite(FAN_RELAY, LOW);   // ON
  } else {
    digitalWrite(FAN_RELAY, HIGH);  // OFF
  }

  // LIGHT LOGIC
  if (motion == HIGH) {
    digitalWrite(LIGHT_RELAY, LOW); // ON
    lastMotionTime = millis();
  }

  if (millis() - lastMotionTime > delayTime) {
    digitalWrite(LIGHT_RELAY, HIGH); // OFF
  }

  delay(2000);
}
