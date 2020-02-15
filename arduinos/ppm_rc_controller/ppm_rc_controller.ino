
/*
 * PPM RC controller
 * 
 * Generates a PPM signal which can be sent to an Graupner/JR module like the FrSky XJT.
 * Usefull to controll an RC model by a computer connected via USB to the microcontroller.
 * 
 * For more details see: https://github.com/asciich/toysautomation
 * 
 * Bases on  https://code.google.com/p/generate-ppm-signal/ 
 * License: GNU General Public License, version 3
 * Written by: reto.hasler@asciich.ch
 */


const int CHANNEL_NUMBER_MIN = 0;
const int CHANNEL_NUMBER_MAX = 8;
const int N_CHANNELS = CHANNEL_NUMBER_MAX - CHANNEL_NUMBER_MIN;

const int PPM_FRAME_LENGTH = 22500;
const int PPM_PULSE_LENGTH = 300;
const int PPM_PULSE_POLARITY = 1;
const int PPM_OUT_PIN = 8; 

#define onState 1//set polarity of the pulses: 1 is positive, 0 is negative

const String SW_TYPE = String("ppm_rc");
const int SW_VERSION_MAJOR = 1;
const int SW_VERSION_MINOR = 0;
const String SW_VERSION_STRING = String(SW_VERSION_MAJOR) + "." + String(SW_VERSION_MINOR);

const int CHANNEL_VALUE_MIN = -100;
const int CHANNEL_VALUE_CENTER = 0;
const int CHANNEL_VALUE_MAX = 100;

const int PPM_VALUE_MIN = 1000;
const int PPM_VALUE_CENTER = 1500;
const int PPM_VALUE_MAX = 2000;


int ppm_values[N_CHANNELS];

void setup_ppm_pin() {
  pinMode(PPM_OUT_PIN, OUTPUT);
  digitalWrite(PPM_OUT_PIN, !onState);
}


void center_all_servos() {
  for (int i = 0; i < N_CHANNELS; i++) {
    ppm_values[i] = PPM_VALUE_CENTER;
  }
}

void setup_ppm_timer() {
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
}

void setup(){  

  Serial.begin(9600);

  setup_ppm_pin();
  center_all_servos();
  setup_ppm_timer();

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
  Serial.write("\n");
}

void serial_list_humanreadable_values() {
  const int buffer_len = 128;
  char write_buffer[buffer_len] = {0,};
  Serial.write("\n");
  
  Serial.write("RC0 values (1000 - 2000):\n");
  for (int i = 0; i < N_CHANNELS; i++) {
    snprintf(write_buffer, buffer_len, "RC0_CH%d = %d\n", i, ppm_values[i]);
    Serial.write(write_buffer);
  }
  Serial.write("\n");
}

void serial_print_help() {
  Serial.write("\n");
  serial_send_string(String("======================\n"));
  serial_send_string(String("PPM RC controller v") + SW_VERSION_STRING);
  serial_send_string(String("======================\n"));
  Serial.write("\n");
  Serial.write("commands:\n");
  serial_send_string(String("  c: Set all channels to center value = \n") + String(CHANNEL_VALUE_CENTER));
  Serial.write("  h: Print this help.\n");
  Serial.write("  lh: List all current states/ values in human readable form.\n");
  serial_send_string(String("  s <N> <VALUE>: Set value for channel <N> where <N> is from ") + String(CHANNEL_NUMBER_MIN) + " to " + String(CHANNEL_NUMBER_MAX) + String("and <VALUE> is from ") + String(CHANNEL_VALUE_MIN) + " to " + String(CHANNEL_VALUE_MAX));
  serial_send_string(String("  t: Return type of this software, returns \"") +  SW_TYPE + "\".\n");
  Serial.write("\n");
}

void serial_send_error(String error_string) {
  serial_send_string(String("!!! ") + error_string + String(" !!!"));
}

bool parse_and_apply_new_channel_position(String channel_and_value_string) {
  int index_of_space = channel_and_value_string.indexOf(' ');
  if (index_of_space == -1) {
    serial_send_error(String("No space found in ") + channel_and_value_string);
    return false;
  }

  if (index_of_space + 1 >= channel_and_value_string.length()) {
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

  if (channel_nr >= N_CHANNELS) {
    serial_send_error(String("Too big channel nr: ") + String(channel_nr));
    return false;
  }

  int servo_value = value_string.toInt();
  if (servo_value < CHANNEL_VALUE_MIN) {
    serial_send_error(String("Too small servo value: ") + String(servo_value));
    return false;
  }

  if (servo_value > CHANNEL_VALUE_MAX) {
    serial_send_error(String("Too big servo value: ") + String(servo_value));
    return false;
  }

  servo_value = map(servo_value, CHANNEL_VALUE_MIN, CHANNEL_VALUE_MAX, PPM_VALUE_MIN, PPM_VALUE_MAX);
  ppm_values[channel_nr] = servo_value;

  return true;
}

void loop(){

  String read_string = read_serial_as_string();
  if (read_string.length() > 0) {
    bool command_success = false;

    if (read_string.equals("c")) {
      center_all_servos();
      command_success = true;
      
    } else if (read_string.equals("h")) {
      serial_print_help();
      command_success = true;
      
    } else if (read_string.equals("lh")) {
      serial_list_humanreadable_values();
      command_success = true;
      
    } else if (read_string.startsWith("s ")) {
      if (parse_and_apply_new_channel_position(read_string.substring(2))) {
        command_success = true;
      }

    } else if (read_string.equals("t")) {
      serial_send_string(SW_TYPE);
      command_success = true;
    }
    
    if (!command_success) {
      serial_print_help();
      serial_send_error(String("Unable to parse \"") + read_string + String("\""));
    }
  }
}

ISR(TIMER1_COMPA_vect){
  /*
   * Many thanks to https://code.google.com/p/generate-ppm-signal/ 
   */
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(PPM_OUT_PIN, onState);
    OCR1A = PPM_PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(PPM_OUT_PIN, !onState);
    state = true;

    if(cur_chan_numb >= N_CHANNELS){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PPM_PULSE_LENGTH;
      OCR1A = (PPM_FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm_values[cur_chan_numb] - PPM_PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm_values[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
