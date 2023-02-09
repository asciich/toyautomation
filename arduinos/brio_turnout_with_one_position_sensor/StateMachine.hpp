#pragma once

typedef void (*fsmCallback)();

void not_initialized_state() {

}

class State {

    private: // members
        String m_state_name = "";
        fsmCallback m_enter_function;

    public: // constructors
        State(String stateName, fsmCallback enterFunction) {
            this->m_state_name = stateName;
            this->m_enter_function = enterFunction;
        }

    public: // Operators
        friend bool operator==( const State& s1, const State& s2 ) {
            return s1.m_enter_function == s2.m_enter_function;
        }

    public: // functions
        void runEnterFunction() {
            (this->m_enter_function)();
        }

        String getStateName() {
            return this->m_state_name;
        }
};

class StateMachine {

    private: // members
        String m_state_machine_name = "";

        State m_previous_state = State("NOT_INITIALIZED_STATE", not_initialized_state) ;
        State m_current_state = State("NOT_INITIALIZED_STATE", not_initialized_state) ;
        State m_next_state = State("NOT_INITIALIZED_STATE", not_initialized_state) ;

        bool m_is_serial_debugging_enabled = false;

    public: // constructors
        StateMachine(String stateMachineName) {
            this->m_state_machine_name = stateMachineName;
        }

    public: // functions

        void enableDebugOutput() {
            this->m_is_serial_debugging_enabled = true;
        }

        String getStateMachineName() {
            return this->m_state_machine_name;
        }

        bool isDebugOutputEnabled() {
            return this->m_is_serial_debugging_enabled;
        }

        void init(State initState) {
            this->m_current_state = initState;
            this->m_next_state = initState;
        }

        bool isCurrentState(State askedState) {
            return this->m_current_state == askedState;
        }

        void transitionTo(State nextState) {
            this->m_next_state = nextState;
        }

        void update() {
            if (this->m_current_state == this->m_next_state) {
                return;
            }

            if (this-isDebugOutputEnabled()) {
                Serial.print("StateMachine '");
                Serial.print(this->getStateMachineName());
                Serial.print("': ");
                Serial.print("Change state from '");
                Serial.print(this->m_current_state.getStateName());
                Serial.print("' to '");
                Serial.print(this->m_next_state.getStateName());
                Serial.println("'");
            }

            this->m_previous_state = this->m_current_state;
            this->m_current_state = this->m_next_state;
            this->m_next_state = this->m_next_state;

            this->m_current_state.runEnterFunction();
        }
};