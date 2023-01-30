// Motor shield schematics: https://www.arduino.cc/en/uploads/Main/arduino_MotorShield_Rev3-schematic.pdf

#include <Stepper.h>

#include "DigitalInput.hpp"
#include "OnboardLED.hpp"
#include "SweepingServo.hpp"
#include "ServoGate.hpp"


// Define number of steps per revolution:
const int stepsPerRevolution = 360 / 1.8;

// Give the motor control pins names:
const int MOTOR_PWM_A_PIN = 3;
const int MOTOR_PWM_B_PIN = 11;
const int MOTOR_BRAKE_A_PIN = 9;
const int MOTOR_BRAKE_B_PIN = 8;
const int MOTOR_DIR_A_PIN = 12;
const int MOTOR_DIR_B_PIN = 13;

const int EJECT_SERVO_PIN = 5; // Orange "out5" on Arduino Motor shield.

const int GATE_SERVO_PIN = 7;
const int GATE_SENSOR_PIN = 10;

// Initialize the stepper library on the motor shield:
Stepper myStepper = Stepper(stepsPerRevolution, MOTOR_DIR_A_PIN, MOTOR_DIR_B_PIN);

SweepingServo ejectServo;
OnboardLED onBoardLED;
DigitalInput elevatorBottomDetector;
DigitalInput elevatorTopDetector;

ServoGate servoGate = ServoGate();


void setup() {
  // Set the PWM and brake pins so that the direction pins can be used to control the motor:
  Serial.begin(9600);

  pinMode(MOTOR_PWM_A_PIN, OUTPUT);
  pinMode(MOTOR_PWM_B_PIN, OUTPUT);
  pinMode(MOTOR_BRAKE_A_PIN, OUTPUT);
  pinMode(MOTOR_BRAKE_B_PIN, OUTPUT);

  stepperCoilOn();

  // Set the motor speed (RPMs):
  myStepper.setSpeed(15);

  ejectServo.attach(EJECT_SERVO_PIN);
  ejectServo.set_to_center();

  elevatorBottomDetector.attach(A3);
  elevatorBottomDetector.setIsInverted(true);
  elevatorTopDetector.attach(A2);
  elevatorTopDetector.setIsInverted(true);
  
  // servoGate.enableSerialDebugging();
  servoGate.attach(GATE_SERVO_PIN, GATE_SENSOR_PIN);
  servoGate.setIsSensorInverted(true);
  servoGate.closeGate();

  onBoardLED.init();
}

void stepperCoilOn() {
  digitalWrite(MOTOR_PWM_A_PIN, HIGH);
  digitalWrite(MOTOR_PWM_B_PIN, HIGH);
  digitalWrite(MOTOR_BRAKE_A_PIN, LOW);
  digitalWrite(MOTOR_BRAKE_B_PIN, LOW);
}

void stepperCoilOff() {
  digitalWrite(MOTOR_PWM_A_PIN, LOW);
  digitalWrite(MOTOR_PWM_B_PIN, LOW);
  digitalWrite(MOTOR_BRAKE_A_PIN, LOW);
  digitalWrite(MOTOR_BRAKE_B_PIN, LOW);
}

void turnStepperSteps( int steps ) {
  stepperCoilOn();
  myStepper.step(steps);
  stepperCoilOff();
}

void readAllInputs() {
  elevatorBottomDetector.read();
  elevatorTopDetector.read();
  servoGate.read();
}

void writeAllOutputs() {
  servoGate.update();
  ejectServo.update();
}

void runAll() {
  servoGate.run();
}

enum stateMachineState {
  SET_ELEVATOR_TO_START_POSITION,
  WAITING_FOR_MARBLE,
  WAIT_FOR_GATE_TO_CLOSE,
  LIFT_ELEVATOR,
  EJECT_MARBLE,
};

volatile stateMachineState currentState = SET_ELEVATOR_TO_START_POSITION;
volatile stateMachineState nextState = SET_ELEVATOR_TO_START_POSITION;

void loop() {
  /*
  turnStepperSteps(stepsPerRevolution);
  
  delay(500);
  
  servo.set_to_min_position();
  servo.update();

  turnStepperSteps(-1 * stepsPerRevolution);

  delay(500);

  servo.set_to_max_position();
  servo.update();
  */

  int loopCounter = 0;
  int loopDelayMillis = 20;
  auto turnsPerIteration = stepsPerRevolution / 8;

  while (true) {
    readAllInputs();
    if (currentState == SET_ELEVATOR_TO_START_POSITION) {
      if (elevatorBottomDetector.isOn()) {
        nextState = WAITING_FOR_MARBLE;
      } else {
        turnStepperSteps(turnsPerIteration);
        loopDelayMillis = 0;
      }
    } else if (currentState == WAITING_FOR_MARBLE) {
      if (servoGate.isSomethingWaiting()) {
        
        servoGate.openGateForNSeconds(0.5);

        nextState = WAIT_FOR_GATE_TO_CLOSE;
      }
    } else if (currentState == WAIT_FOR_GATE_TO_CLOSE) {
      if (! servoGate.isRunning()) {
        nextState = LIFT_ELEVATOR;
      }
    } else if (currentState == LIFT_ELEVATOR) {
      if (elevatorTopDetector.isOn()) {
        nextState = EJECT_MARBLE;
      } else {
        turnStepperSteps(-1 * turnsPerIteration);
        loopDelayMillis = 0;
      }
    }else if (currentState == EJECT_MARBLE) {
      ejectServo.write(0);
      ejectServo.update();
      delay(1000);
      ejectServo.set_to_center();
      ejectServo.update();
      nextState = SET_ELEVATOR_TO_START_POSITION;
    }

    runAll();

    writeAllOutputs();

    if (currentState != nextState) {
      Serial.print("Next state: ");
      if (nextState == SET_ELEVATOR_TO_START_POSITION) {
        Serial.println("SET_ELEVATOR_TO_START_POSITION");
      } else if (nextState == WAITING_FOR_MARBLE) {
        Serial.println("WAITING_FOR_MARBLE");
      } else if (nextState == WAIT_FOR_GATE_TO_CLOSE) {
        Serial.println("WAIT_FOR_GATE_TO_CLOSE");
      } else if (nextState == LIFT_ELEVATOR) {
        Serial.println("LIFT_ELEVATOR");
      } else if (nextState == EJECT_MARBLE) {
        Serial.println("EJECT_MARBLE");
      } else {
        Serial.println("Unknown state");
      }
    }

    loopCounter += 1;
    if (loopCounter %100 == 0 ) {
      Serial.print("current state: ");
      if (nextState == SET_ELEVATOR_TO_START_POSITION) {
        Serial.println("SET_ELEVATOR_TO_START_POSITION");
      } else if (currentState == WAITING_FOR_MARBLE) {
        Serial.println("WAITING_FOR_MARBLE");
      } else if (currentState == WAIT_FOR_GATE_TO_CLOSE) {
        Serial.println("WAIT_FOR_GATE_TO_CLOSE");
      } else if (nextState == LIFT_ELEVATOR) {
        Serial.println("LIFT_ELEVATOR");
      } else if (nextState == EJECT_MARBLE) {
        Serial.println("EJECT_MARBLE");
      } else {
        Serial.println("Unknown state");
      }
    }

    currentState = nextState;

    delay(loopDelayMillis);

  }
}
