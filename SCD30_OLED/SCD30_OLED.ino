#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2cScd30.h>
#include "SSD1306Wire.h"

SensirionI2cScd30 sensor;
SSD1306Wire display(0x3c, SDA, SCL);

static char errorMessage[128];
static int16_t error;

void setup() {

    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }

    Wire.begin();

    display.init();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 22, "PATIENCE");
    display.display();
    
    sensor.begin(Wire, SCD30_I2C_ADDR_61);

    sensor.stopPeriodicMeasurement();
    sensor.softReset();
    delay(2000);

    error = sensor.startPeriodicMeasurement(0);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
}

void loop() {

    float co2Concentration = 0.0;
    float temperature = 0.0;
    float humidity = 0.0;
    delay(1500);
    error = sensor.blockingReadMeasurementData(co2Concentration, temperature,
                                               humidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute blockingReadMeasurementData(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print(co2Concentration);
    Serial.print("\t");
    Serial.print(temperature);
    Serial.print("\t");
    Serial.print(humidity);
    Serial.println();

    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 12, String(int(round(co2Concentration))));

    
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(4, 16, "CO");
    
    display.setFont(ArialMT_Plain_10);
    display.drawString(28, 24, "2");


    display.drawString(4, 52, "t  " + String(int(round(temperature))) + " C");

    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(124, 16, "PPM");

    display.drawString(124, 52, "hum  " + String(int(round(humidity))) + "%");


    display.display();

}
