#include <Servo.h>

#pragma once

class SweepingServo {
  private: // contstants:
    const int TOGGLE_STATUS_IDLE = 0;
    const int TOGGLE_STATUS_POS1 = 1;
    const int TOGGLE_STATUS_POS2 = 2;
  
  private: // members
    Servo m_servo;
    int m_pin_number = -1;

    int m_current_position = 90;
    int m_target_position = 90;
    int m_position_step = 2;

    int m_max_position = 170; // not all servos support 180 degrees angle
    int m_min_position = 10; // not all servos support 0 degrees angle
    int m_center_psoition = 90;

    int m_toggle_position_1 = -1;
    int m_toggle_position_2 = -1;
    int m_toggle_step = TOGGLE_STATUS_IDLE;

  public: // functions
    void attach(const int pin_number) {
      this->m_servo.attach(pin_number);
      this->m_pin_number = pin_number;
    }

    int getPinNumber() {
      return this->m_pin_number;
    }

    /*
      Set servo to center position.
    */
    void set_to_center() {
      this->write(this->m_center_psoition);
    }

    void set_to_min_position() {
      this->write(this->m_min_position);
    }

    void set_to_max_position() {
      this->write(this->m_max_position);
    }

    void write(int new_position_degrees) {
      this->m_target_position = new_position_degrees;
    }

    int read() {
      return this->m_current_position;
    }

    bool is_position_reached() {
      if (this->m_current_position == this->m_target_position) {
        return true;
      } else {
        return false;
      }
    }
    
    void update() {
      if (this->is_position_reached()) {
        return;
      }
      
      this->m_servo.write(this->m_target_position);
      this->m_current_position = this->m_target_position;
    }
};
