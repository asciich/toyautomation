#pragma once

class DigitalInput {

  private: // members:
    int m_pin_number = -1;
    int m_last_value = false;
    int m_value = false;
    bool m_is_inverted = false;
    unsigned long m_millis_changed = 0;
    unsigned long m_minimum_hold_after_change_millis = 0;
    

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

    bool isMinimumHoldAfterChangeMillisSet() {
      return this->m_minimum_hold_after_change_millis > 0;
    }

    /**
      Set minimum hold time after a value change in milli seconds.
      Useful to debounce inputs.
    */
    void setMinumumHoldAfterChangeMillis(unsigned long minimum_hold) {
      this->m_minimum_hold_after_change_millis = minimum_hold;
    }

    unsigned long getChangedSinceMillis() {
      return millis() - this->m_millis_changed;
    }

    bool read() {
      if (this->is_attached()) {
        this->m_last_value = this->m_value;
        if (this->isMinimumHoldAfterChangeMillisSet()) {
          if (this->getChangedSinceMillis() >= this->m_minimum_hold_after_change_millis) {
            this->m_value = digitalRead(this->m_pin_number);  
          }
        } else {
          this->m_value = digitalRead(this->m_pin_number);
        }
      } else {
        this->m_last_value = false;
        this->m_value = false;
      }

      if (this->isValueChanged()) {
        this->m_millis_changed = millis();
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

    void setInverted(bool isInverted) {
      this->setIsInverted(isInverted);
    }

    void setIsInverted(bool isInverted) {
      this->m_is_inverted = isInverted;
    }
}; 
