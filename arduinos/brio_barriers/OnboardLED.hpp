/*
 Simplified access to set the onboard LED.
 */

#pragma once

class OnboardLED {

    public: // functions
        
        void init() {
            // initialize digital pin LED_BUILTIN as an output.
            pinMode(LED_BUILTIN, OUTPUT);
        }

        void onDirectWrite() {
            this->setDirectWrite(true);
        }

        void offDirectWrite() {
            this->setDirectWrite(false);
        }

        void setDirectWrite(bool ledValue) {
            if (ledValue) {
                digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
            } else {
                digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
            }
        }
};