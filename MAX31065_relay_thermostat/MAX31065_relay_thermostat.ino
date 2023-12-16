#include <ESP8266WiFi.h>
#include <Adafruit_MAX31865.h>
#include <ThingSpeak.h>

/*
  Credentials.h should contains the following constants:

  const char* WIFI_SSID = "Your WiFi network SSID (name)";
  const char* WIFI_PASSWORD = "Your WiFi password";
  unsigned long THINGSPEAK_CHANNEL_ID = <Your ThingSpeak Channel ID>;
  const char* THINGSPEAK_API_KEY = "Your ThingSpeak Write API  Key";
*/
#include "Credentials.h"

Adafruit_MAX31865 thermo = Adafruit_MAX31865(12, 13, 14, 15);

#define RREF      430.0
#define RNOMINAL  100.0

#define MAX_TEMP  26.5
#define MIN_TEMP  24.5

#define RELAY_PIN D1

#define UPDATE_FREQUENCY      120000
#define WIFI_RECONNECT_DELAY  500

#define DEBUG_ESP_WIFI
#define DEBUG_ESP_PORT

WiFiClient client;

void setup() {

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  ThingSpeak.begin(client);
  thermo.begin(MAX31865_3WIRE);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  

  Serial.print("\nConnecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(WIFI_RECONNECT_DELAY);
  } 
  Serial.println("OK");

  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  WiFi.hostname("MAX31065-Thermorelay");
}

void loop() {

  float t = thermo.temperature(RNOMINAL, RREF);

  int httpStatus = ThingSpeak.writeField(THINGSPEAK_CHANNEL_ID, 1, t, THINGSPEAK_API_KEY);

  if (httpStatus != 200) {
    Serial.println("ThingSpeak t° HTTP error: " + String(httpStatus));
  }
  
  if (t <= MIN_TEMP) {
  
    digitalWrite(RELAY_PIN, LOW);   // Relay ON
    digitalWrite(LED_BUILTIN, LOW); // LED ON

  } else if(t > MAX_TEMP) {
  
    digitalWrite(RELAY_PIN, HIGH);    // Relay OFF
    digitalWrite(LED_BUILTIN, HIGH);  // LED OFF
  }

  Serial.print("t°: "); 
  Serial.println(t);
  
  delay(UPDATE_FREQUENCY);
}
