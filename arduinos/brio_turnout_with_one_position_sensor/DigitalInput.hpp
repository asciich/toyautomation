#pragma once

class DigitalInput {

  private: // members:
    int m_pin_number = -1;
    int m_last_value = false;
    int m_value = false;
    bool m_is_inverted = false;

  public: // destructor
    virtual ~DigitalInput() {}

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
        this->m_last_value = this->m_value;
        this->m_value = digitalRead(this->m_pin_number);
      } else {
        this->m_last_value = false;
        this->m_value = false;
      }

      return m_value;
    }

    int getPinNumber() {
      return this->m_pin_number;
    }
    
    bool getValue() {
        return this->m_value;
    }

    bool isOn() {
      if (this->isInverted()) {
        return !this->getValue();
      } else {
        return this->getValue();
      }
    }

    bool isOff() {
      if (this->isInverted()) {
        return this->getValue();
      } else {
        return !this->getValue();
      }
    }

    bool isInverted() {
      return this->m_is_inverted;
    }

    bool isValueChanged() {
      return this->m_value != this->m_last_value;
    }

    void setIsInverted(bool isInverted) {
      this->m_is_inverted = isInverted;
    }
}; 
