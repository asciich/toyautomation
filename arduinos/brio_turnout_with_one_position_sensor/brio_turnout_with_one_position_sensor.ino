#include "DigitalInput.hpp"
#include "SweepingServo.hpp"
#include "OnboardLED.hpp"
#include "StateMachine.hpp"

DigitalInput positionSensor;
SweepingServo turnoutServo;
OnboardLED onBoardLED;
StateMachine turnoutStateMachine = StateMachine("TURNOUT_WITH_ONE_POSITION_SENSOR");

void enterIdle() {
    onBoardLED.offDirectWrite();
}

void enterToggle() {
    onBoardLED.onDirectWrite();
    turnoutServo.toggle_between_min_and_max_position();
}

void enterWaitPositionSensorClear() {
    onBoardLED.onDirectWrite();
}

State STATE_IDLE = State("IDLE", enterIdle);
State STATE_TOGGLE = State("TOGGLE", enterToggle);
State STATE_WAIT_POSITION_SENSOR_CLEAR = State("WAIT_FOR_POSITION_SENSOR_CLEAR", enterWaitPositionSensorClear);

void setup() {
    Serial.begin(9600);

    positionSensor.attach(2);
    
    turnoutServo.attach(3);
    turnoutServo.trim(75, 90, 105);
    turnoutServo.set_to_min_position();

    onBoardLED.init();

    turnoutStateMachine.init(STATE_IDLE);
}

void readAllInputs() {
    positionSensor.read();
}

void updateAll() {
    turnoutServo.update();
}

void loop() {
    readAllInputs();
    turnoutStateMachine.update();

    if (turnoutStateMachine.isCurrentState(STATE_IDLE)) {
        if (positionSensor.isOn()) {
            turnoutStateMachine.transitionTo(STATE_TOGGLE);
        }
    } else if (turnoutStateMachine.isCurrentState(STATE_TOGGLE)) {
        turnoutStateMachine.transitionTo(STATE_WAIT_POSITION_SENSOR_CLEAR);
    } else if (turnoutStateMachine.isCurrentState(STATE_WAIT_POSITION_SENSOR_CLEAR)) {
        if (positionSensor.isOff()) {
            turnoutStateMachine.transitionTo(STATE_IDLE);
        }
    }

    updateAll();

    delay(10);
}