#include <Arduino.h>

#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x20,16,2);

#include "MessageProcessor.h"
MessageProcessor processor(&lcd);

void setup() {
    Serial.begin(57600);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Ready");
}

void loop() {
    processor.process(Serial);
}
