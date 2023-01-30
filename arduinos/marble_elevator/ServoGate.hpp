/*
 * ServoGate.hpp
 * 
 * A servo gate is a gate wich can be opened and closed by a servo.
 * Furthermore there is an indicator if someone or something is waiting to pass the gate.
*/

#include "SweepingServo.hpp"
#include "NonBlockingTimer.hpp"
#include "DigitalInput.hpp"

#pragma once

class ServoGate {

    private: // members
        SweepingServo m_gate_servo;
        DigitalInput m_sensor_pin;
        enum stateMachineState {
            DIRECT_OPERATION,
            OPEN_AND_CLOSE_GATE_WAIT_OPEN
        };

        stateMachineState m_current_state = DIRECT_OPERATION;
        stateMachineState m_next_state = DIRECT_OPERATION;

        NonBlockingTimer m_keepOpenTimer;

        bool m_is_serial_debugging_enabled = false;

        int loop_counter = 0;

    public: // functions
        void attach(int servoPin, int sensorPin) {
            this->m_gate_servo.attach(servoPin);
            this->m_sensor_pin.attach(sensorPin);

            if (this->isSerialDebuggingEnabled()) {
                Serial.print("ServoGate: Attached gate servo pin: ");
                Serial.println(this->m_gate_servo.getPinNumber());
                Serial.print("ServoGate: Attached gate sensor pin: ");
                Serial.println(this->m_sensor_pin.getPinNumber());
            }
        }

        void closeGate() {
            this->m_gate_servo.write(90);
            if (this->isSerialDebuggingEnabled()) {
                Serial.println("ServoGate: closeGate().");
            }
        }

        void enableSerialDebugging() {
            this->m_is_serial_debugging_enabled = true;
            this->m_keepOpenTimer.enableSerialDebugging();
            Serial.println("ServoGate: Serial debugging enabled.");
        }

        bool isSerialDebuggingEnabled() {
            return this->m_is_serial_debugging_enabled;
        }

        void openGate() {
            this->m_gate_servo.write(0);
            if (this->isSerialDebuggingEnabled()) {
                Serial.println("ServoGate: openGate().");
            }
        }

        void openGateForNSeconds(float openDuration) {
            this->openGate();
            this->m_keepOpenTimer.startForNSeconds(openDuration);
            this->m_next_state = OPEN_AND_CLOSE_GATE_WAIT_OPEN;
            if (this->isSerialDebuggingEnabled()) {
                Serial.print("ServoGate: openGateForNSeconds = ");
                Serial.print(openDuration);
                Serial.println(" started.");
            }
        }

        void run() {
            if (this->m_current_state == DIRECT_OPERATION) {

            } else if (this->m_current_state == OPEN_AND_CLOSE_GATE_WAIT_OPEN) {
                if ( this->m_keepOpenTimer.isElapsed() ) {
                    this->m_keepOpenTimer.reset();
                    this->closeGate();
                    this->m_next_state = DIRECT_OPERATION;
                }
            }

            if (this->m_next_state != this->m_current_state) {
                if (this->isSerialDebuggingEnabled()) {
                    Serial.print("ServoGate: next state: ");
                    logStateToSerial();
                }
            }

            this->m_current_state = this->m_next_state;

            loop_counter += 1;
            if (this->isSerialDebuggingEnabled()) {
                if (loop_counter % 100 == 0 ) {
                    Serial.print("ServoGate: current state: ");
                    logStateToSerial();
                }
            }

        }

        void logStateToSerial() {
            if (this->m_next_state == DIRECT_OPERATION) {
                Serial.println("DIRECT_OPERATION");
            } else if (this->m_next_state == OPEN_AND_CLOSE_GATE_WAIT_OPEN) {
                Serial.println("OPEN_AND_CLOSE_GATE_WAIT_OPEN");
            } else {
                Serial.println("Unknown state.");
            }
        }

        void update() {
            this->m_gate_servo.update();
        }
        
        void read() {
            this->m_sensor_pin.read();
            if (this->isSerialDebuggingEnabled()) {
                if (this->m_sensor_pin.isValueChanged()) {
                    Serial.print("ServoGate: Sensor value changed. IsSomethingWaiting() = ");
                    Serial.println(this->isSomethingWaiting());
                }
            }
        }

        bool isRunning() {
            if (this->m_current_state != DIRECT_OPERATION) {
                return true;
            }
            
            return this->m_keepOpenTimer.isRunning();
        }

        bool isSensorInverted() {
            return this->m_sensor_pin.isInverted();
        }

        bool isSomethingWaiting() {
            return this->m_sensor_pin.isOn();
        }

        void setIsSensorInverted(bool isSensorInverted) {
            return this->m_sensor_pin.setIsInverted(isSensorInverted);
        }
};
