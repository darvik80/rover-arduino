#include <Arduino.h>


//#include "LiquidCrystal_I2C.h"
#include "ArduinoJson.h"

#undef min
#undef max

#include "etl/crc16.h"

//LiquidCrystal_I2C lcd(0x20,16,2);

void setup() {
    Serial.begin(57600);

    /*
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Ready...");
     */

    StaticJsonDocument<200> doc;
    doc["sensor"] = "gps";
    doc["time"] = 1351824120;

    String result;
    serializeJson(doc, result);

    etl::crc16 crc;
    crc.add(0xff);
    crc.value();
}

void loop() {

}
