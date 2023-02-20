const int GREEN_LED_PIN = 3;
const int RED_LED_PIN = 2; 

const int GREEN_PHASE_SECONDS = 5;
const int RED_PHASE_SECONDS = 10;

const int PIN_VALUE_ON = LOW; // minus of the LED is connected to the input pin, so it is inverted!
const int PIN_VALUE_OFF = HIGH; // minus of the LED is connected to the input pin, so it is inverted!

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
}

void builtinLedOn() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void builtinLedOff() {
  digitalWrite(LED_BUILTIN, LOW);
}

void turnGreen() {
  digitalWrite(GREEN_LED_PIN, PIN_VALUE_ON);
  digitalWrite(RED_LED_PIN, PIN_VALUE_OFF);
  builtinLedOn();
}

void turnRed() {
  digitalWrite(GREEN_LED_PIN, PIN_VALUE_OFF);
  digitalWrite(RED_LED_PIN, PIN_VALUE_ON);
  builtinLedOff();
}

void loop() {
  turnGreen();
  delay(1000 * GREEN_PHASE_SECONDS);
  turnRed();
  delay(1000 * RED_PHASE_SECONDS);
}
