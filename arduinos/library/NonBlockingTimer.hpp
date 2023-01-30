#pragma once

class NonBlockingTimer {

    private: // members
        unsigned long m_t_start = -1;
        unsigned long m_wait_for_millis = -1;

        bool m_is_serial_debugging_enabled = false;

    public: // functions
        
        void enableSerialDebugging() {
            this->m_is_serial_debugging_enabled = true;
            Serial.println("NonBlockingTimer: Serial debugging enabled.");
        }

        void startForNSeconds(float seconds) {
            this->m_t_start = millis();
            this->m_wait_for_millis = int(seconds * 1000);
            if (this->isSerialDebuggingEnabled()) {
                Serial.print("NonBlockingTimer: startForNSeconds = ");
                Serial.print(seconds);
                Serial.print("s = ");
                Serial.print(this->m_wait_for_millis);
                Serial.println("ms started.");
            }
        }
 
        bool isSerialDebuggingEnabled() {
            return this->m_is_serial_debugging_enabled;
        }

        bool isRunning() {
            if (this->m_wait_for_millis < 0) {
                if (this->isSerialDebuggingEnabled()) {
                    Serial.println("NonBlockingTimer: isRunning is false since m_wait_for_millis < 0");
                }
                return false;
            }


            bool isRunningValue = !this->isElapsed();
            return isRunningValue;
        }

        bool isElapsed() {
            if (this->m_t_start < 0) {
                return false;
            }

            unsigned long t_now = millis();
            long difference = (unsigned long)(t_now - this->m_t_start) - this->m_wait_for_millis;
            if (this->isSerialDebuggingEnabled()) {
                Serial.print("NonBlockingTimer: t_now=");
                Serial.print(t_now);
                Serial.print(" m_t_start=");
                Serial.print(this->m_t_start);
                Serial.print(" m_wait_for_millis=");
                Serial.print(this->m_wait_for_millis);
                Serial.print(" difference=");
                Serial.println(difference);
            }
            return difference > 0;
        }

        void reset() {
            this->m_t_start = -1;
            this->m_wait_for_millis = -1;
            if (this->isSerialDebuggingEnabled()) {
                Serial.print("NonBlockingTimer: resetted.");
            }
        }
};