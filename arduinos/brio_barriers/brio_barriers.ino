#include "DigitalInput.hpp"
#include "SweepingServo.hpp"
#include "OnboardLED.hpp"
#include "StateMachine.hpp"

DigitalInput trainSensor0;
DigitalInput trainSensor1;

SweepingServo barrierServo0;
SweepingServo barrierServo1;

OnboardLED onBoardLED;
StateMachine barriersStateMachine = StateMachine("BRIO_BARRIERS");

void enterBarriersOpen() {
    onBoardLED.onDirectWrite();

    barrierServo0.set_to_min_position();
    barrierServo1.set_to_max_position();
}

void enterBarriersOpenWaitClear() {
    enterBarriersOpen();
}

void enterBarriersClose() {
    onBoardLED.offDirectWrite();

    barrierServo0.set_to_center();
    barrierServo1.set_to_center();
}

void enterBarriersCloseWaitClear() {
    enterBarriersClose();
}

State STATE_BARRIERS_OPEN = State("BARRIERS_OPEN_WAIT_CLEAR", enterBarriersOpen);
State STATE_BARRIERS_OPEN_WAIT_CLEAR = State("BARRIERS_OPEN", enterBarriersOpenWaitClear);
State STATE_BARRIERS_CLOSE = State("BARRIERS_CLOSE", enterBarriersClose);
State STATE_BARRIERS_CLOSE_WAIT_CLEAR = State("BARRIERS_CLOSE_WAIT_CLEAR", enterBarriersCloseWaitClear);

void setup() {
    Serial.begin(9600);

    trainSensor0.attach(2);
    trainSensor0.setInverted(true);
    trainSensor0.setMinumumHoldAfterChangeMillis(500);
    trainSensor1.attach(3);
    trainSensor1.setInverted(true);
    trainSensor1.setMinumumHoldAfterChangeMillis(500);
    
    barrierServo0.attach(4);
    barrierServo0.trim(0, 90, 180);
    barrierServo0.set_to_min_position();

    barrierServo1.attach(5);
    barrierServo1.trim(0, 90, 180);
    barrierServo1.set_to_max_position();
    
    onBoardLED.init();

    barriersStateMachine.init(STATE_BARRIERS_OPEN);
}

void readAllInputs() {
    trainSensor0.read();
    trainSensor1.read();

    if (trainSensor0.isValueChanged() || trainSensor1.isValueChanged()) {
        Serial.print("sensor 0: ");
        Serial.print(trainSensor0.isOn());
        Serial.print("sensor 1: ");
        Serial.print(trainSensor1.isOn());
        Serial.println(".");
    }
}

void updateAll() {
    barrierServo0.update();
    barrierServo1.update();
}

void loop() {
    readAllInputs();
    barriersStateMachine.update();
    

    if (barriersStateMachine.isCurrentState(STATE_BARRIERS_OPEN)) {
        if (trainSensor0.isOn() || trainSensor1.isOn()) {
            barriersStateMachine.transitionTo(STATE_BARRIERS_CLOSE_WAIT_CLEAR);
        }
    } else if (barriersStateMachine.isCurrentState(STATE_BARRIERS_CLOSE_WAIT_CLEAR)) {
        if (trainSensor0.isOff() && trainSensor1.isOff()) {
            barriersStateMachine.transitionTo(STATE_BARRIERS_CLOSE);
        }
    } else if (barriersStateMachine.isCurrentState(STATE_BARRIERS_CLOSE)) {
        if (trainSensor0.isOn() || trainSensor1.isOn()) {
            barriersStateMachine.transitionTo(STATE_BARRIERS_OPEN_WAIT_CLEAR);
        }
    } else if (barriersStateMachine.isCurrentState(STATE_BARRIERS_OPEN_WAIT_CLEAR)) {
        if (trainSensor0.isOff() && trainSensor1.isOff()) {
            barriersStateMachine.transitionTo(STATE_BARRIERS_OPEN);
        }
    } else {
        Serial.println("Unknown state.");
    }

    updateAll();

    delay(10);
}