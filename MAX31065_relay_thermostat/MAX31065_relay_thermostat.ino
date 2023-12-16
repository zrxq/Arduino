#include <Adafruit_MAX31865.h>
#include <ESP8266WiFi.h>

Adafruit_MAX31865 thermo = Adafruit_MAX31865(12, 13, 14, 15);

#define RREF      430.0
#define RNOMINAL  100.0

#define MAX_TEMP  26.5
#define MIN_TEMP  24.5

#define RELAY_PIN D1

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  thermo.begin(MAX31865_3WIRE);
}

void loop() {
  float t = thermo.temperature(RNOMINAL, RREF);

  if(t <= MIN_TEMP) {
    digitalWrite(RELAY_PIN, LOW); // Relay ON
    digitalWrite(LED_BUILTIN, LOW); // LED ON
    Serial.print("Heating ON\n");
  } else if(t > MAX_TEMP ) {
    digitalWrite(RELAY_PIN, HIGH); // Relay OFF
    digitalWrite(LED_BUILTIN, HIGH); // LED OFF
    Serial.print("Heating OFF\n");
  }

  Serial.print("Temperature = "); Serial.println(t);
  delay(5000);
}
