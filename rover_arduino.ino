#include <Arduino.h>
#include "User_Setup.h"

void setup() {
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        Serial.println(Serial.readString());
    }
}
