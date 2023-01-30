#include "SweepingServo.hpp"

/**
 * Toy automation lenoardo servo
 */

const String SW_TYPE = "servo";
const int SW_VERSION_MAJOR = 1;
const int SW_VERSION_MINOR = 0;
const String SW_VERSION_STRING = String(SW_VERSION_MAJOR) + "." + String(SW_VERSION_MINOR);

const int N_SENSOR_IN_CHANNELS = 6;
const int SENSOR_IN_PIN_0 = 8;
const int SENSOR_IN_PIN_1 = 9;
const int SENSOR_IN_PIN_2 = 10;
const int SENSOR_IN_PIN_3 = 11;
const int SENSOR_IN_PIN_4 = 12;
const int SENSOR_IN_PIN_5 = 13;

const int N_SERVO_OUT_CHANNELS = 8;
const int SERVO_OUT_PIN_0 = 0;
const int SERVO_OUT_PIN_1 = 1;
const int SERVO_OUT_PIN_2 = 2;
const int SERVO_OUT_PIN_3 = 3;
const int SERVO_OUT_PIN_4 = 4;
const int SERVO_OUT_PIN_5 = 5;
const int SERVO_OUT_PIN_6 = 6;
const int SERVO_OUT_PIN_7 = 7;

const int SERVO_VALUE_MIN = 0;
const int SERVO_VALUE_MAX = 180;
const int SERVO_VALUE_CENTER = 90;

class SensorInput {

  private: // members:
    int m_pin_number = -1;

  public: // destructor
    virtual ~SensorInput() {}

  public: // functions

    void attach(int pin_number) {
      pinMode(pin_number, INPUT);
      this->m_pin_number = pin_number;
    }

    bool is_attached() {
      if ( this->m_pin_number == -1 ) {
        return false;
      } else {
        return true;
      }
    }

    bool read() {
      if (this->is_attached()) {
        return digitalRead(this->m_pin_number);
      } else {
        return false;
      }
    }
    
};

SweepingServo servos_out[N_SERVO_OUT_CHANNELS];
SensorInput sensors_in[N_SENSOR_IN_CHANNELS];

void setup(){  

  Serial.begin(9600);

  servos_out[0].attach(SERVO_OUT_PIN_0);
  servos_out[1].attach(SERVO_OUT_PIN_1);
  servos_out[2].attach(SERVO_OUT_PIN_2);
  servos_out[3].attach(SERVO_OUT_PIN_3);
  servos_out[4].attach(SERVO_OUT_PIN_4);
  servos_out[5].attach(SERVO_OUT_PIN_5);
  servos_out[6].attach(SERVO_OUT_PIN_6);
  servos_out[7].attach(SERVO_OUT_PIN_7); 

  sensors_in[0].attach(SENSOR_IN_PIN_0);
  sensors_in[1].attach(SENSOR_IN_PIN_1);
  sensors_in[2].attach(SENSOR_IN_PIN_2);
  sensors_in[3].attach(SENSOR_IN_PIN_3);
  sensors_in[4].attach(SENSOR_IN_PIN_4);
  sensors_in[5].attach(SENSOR_IN_PIN_5);
}


String read_serial_as_string() {
  static String read_buffer = String("");

  char c = 0x00;

  while (Serial.available()) {
    c = Serial.read();
    if (c != '\n') {
      read_buffer += c;
    }
  }

  if (c == '\n') {
    String ret_val = String(read_buffer);
    read_buffer = String("");
    return ret_val;  
  } else {
    return String(""); 
  }
}

void serial_send_string(String to_send) {
  Serial.write(to_send.c_str(), to_send.length());
  if (to_send.endsWith("\n")) {
    return;
  } else {
    Serial.write("\n");
  }
}

void serial_list_humanreadable_values() {
  const int buffer_len = 128;
  char write_buffer[buffer_len] = {0,};
  Serial.write("\n");

  Serial.write("\n Sensor input values:\n");
  for (int i = 0; i < N_SENSOR_IN_CHANNELS; i++) {
    serial_send_string(String("  Sensor ") + String(i) + " = " + String(sensors_in[i].read()));
  }
  Serial.write("\n");

  Serial.write("\n Servo output values:\n");
  for (int i = 0; i < N_SERVO_OUT_CHANNELS; i++) {
    serial_send_string(String("  Servo ") + String(i) + " = " + String(servos_out[i].read()));
  }
  Serial.write("\n");
}

void serial_print_help() {
  Serial.write("\n");
  Serial.write("=======================\n");
  Serial.write("servo and sensor \n");
  Serial.write("=======================\n");
  Serial.write("\n");
  Serial.write("commands:\n");
  Serial.write("  h : Print this help.\n");
  Serial.write("  lh : List all current states/ values in human readable form.\n");
  serial_send_string(String("  t: Return type of this software, returns \"") +  SW_TYPE + "\".\n");
  serial_send_string(String("  r: Read digital inputs. Currently ") + String(N_SENSOR_IN_CHANNELS) + String(" digital sensor will be read.\n"));
  serial_send_string(String("  s : Set a servo value. s <SERVO_NR> <POSITION> where SERVO_NR is an Integer and Position is an integer from ") + String(SERVO_VALUE_MIN) + " to " + String(SERVO_VALUE_MAX) + " degrees.");
  Serial.write("  sc: Set all servos to center position");
  serial_send_string(String("  v: Return software version ") +  SW_VERSION_STRING + "\n");
  Serial.write("\n");
}


void update_servo_outputs() {
  for (int i = 0; i < N_SERVO_OUT_CHANNELS; i++) {
    servos_out[i].update();
  }
}

void center_all_servos() {
  for (int i = 0; i < N_SERVO_OUT_CHANNELS; i++) {
    servos_out[i].write(SERVO_VALUE_CENTER);
  }
}

void serial_send_error(String error_string) {
  serial_send_string(String("!!! ") + error_string + String(" !!!"));
}

bool parse_and_apply_new_servo_position(String channel_and_value_string) {
  int index_of_space = channel_and_value_string.indexOf(' ');
  if (index_of_space == -1) {
    serial_send_error(String("No space found in ") + channel_and_value_string);
    return false;
  }

  if (index_of_space + 2 >= channel_and_value_string.length()) {
    serial_send_error(String("channel_and_value_string to short"));
    return false;
  }

  String channel_string = channel_and_value_string.substring(0, index_of_space);
  String value_string = channel_and_value_string.substring(index_of_space + 1);

  int channel_nr = channel_string.toInt();
  if (channel_nr < 0) {
    serial_send_error(String("Too small channel nr: ") + String(channel_nr));
    return false;
  }

  if (channel_nr >= N_SERVO_OUT_CHANNELS) {
    serial_send_error(String("Too big channel nr: ") + String(channel_nr));
    return false;
  }

  int servo_value = value_string.toInt();
  if (servo_value < SERVO_VALUE_MIN) {
    serial_send_error(String("Too small servo value: ") + String(servo_value));
    return false;
  }

  if (servo_value > SERVO_VALUE_MAX) {
    serial_send_error(String("Too big servo value: ") + String(servo_value));
    return false;
  }

  servos_out[channel_nr].write(servo_value);

  return true;
}

void send_sensor_inputs() {
  char values = 0x00;
  
  for (int i = 0; i < N_SENSOR_IN_CHANNELS; i++) {
    values |= sensors_in[i].read() << i;
  }

  String hex_string = String(values, HEX);
  if (hex_string.length() == 1) {
    hex_string = String("0") + hex_string;
  }
  serial_send_string(hex_string);
}

void loop(){

  String read_string = read_serial_as_string();
  if (read_string.length() > 0) {
    bool command_success = false;
    
    if (read_string.equals("h")) {
      serial_print_help();
      command_success = true;

    } else if (read_string.equals("r")) {
      send_sensor_inputs();
      command_success = true;
    
    } else if (read_string.startsWith("s ")) {
      if (parse_and_apply_new_servo_position(read_string.substring(2))) {
        command_success = true;
      }
      
    } else if (read_string.equals("sc")) {
      center_all_servos();
      command_success = true;

    } else if (read_string.equals("t")) {
      serial_send_string(SW_TYPE);
      command_success = true;
      
    } else if (read_string.equals("v")) {
      serial_send_string(SW_VERSION_STRING);
      command_success = true;
      
    } else if (read_string.equals("lh")) {
      serial_list_humanreadable_values();
      command_success = true;
      
    } 

    if (!command_success) {
      serial_print_help();
      serial_send_error(String("Unable to parse \"") + read_string + String("\""));
    }
  }

  update_servo_outputs();
}

