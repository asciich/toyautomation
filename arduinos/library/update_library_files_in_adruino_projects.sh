#!/usr/bin/env bash

set -e


SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd ${SCRIPT_DIR}
SCRIPT_NAME="$(basename -- "${BASH_SOURCE[0]}" )"

LIBRARY_DIR="${SCRIPT_DIR}"
ARDUINOS_DIR="$( dirname "${SCRIPT_DIR}" )"

cp -v "${LIBRARY_DIR}/DigitalInput.hpp" "${ARDUINOS_DIR}/marble_elevator/."
cp -v "${LIBRARY_DIR}/NonBlockingTimer.hpp" "${ARDUINOS_DIR}/marble_elevator/."
cp -v "${LIBRARY_DIR}/OnboardLED.hpp" "${ARDUINOS_DIR}/marble_elevator/."
cp -v "${LIBRARY_DIR}/SweepingServo.hpp" "${ARDUINOS_DIR}/marble_elevator/."
cp -v "${LIBRARY_DIR}/ServoGate.hpp" "${ARDUINOS_DIR}/marble_elevator/."

cp -v "${LIBRARY_DIR}/DigitalInput.hpp" "${ARDUINOS_DIR}/brio_barriers/."
cp -v "${LIBRARY_DIR}/OnboardLED.hpp" "${ARDUINOS_DIR}/brio_barriers/."
cp -v "${LIBRARY_DIR}/StateMachine.hpp" "${ARDUINOS_DIR}/brio_barriers/."
cp -v "${LIBRARY_DIR}/SweepingServo.hpp" "${ARDUINOS_DIR}/brio_barriers/."

cp -v "${LIBRARY_DIR}/DigitalInput.hpp" "${ARDUINOS_DIR}/brio_turnout_with_one_position_sensor/."
cp -v "${LIBRARY_DIR}/OnboardLED.hpp" "${ARDUINOS_DIR}/brio_turnout_with_one_position_sensor/."
cp -v "${LIBRARY_DIR}/StateMachine.hpp" "${ARDUINOS_DIR}/brio_turnout_with_one_position_sensor/."
cp -v "${LIBRARY_DIR}/SweepingServo.hpp" "${ARDUINOS_DIR}/brio_turnout_with_one_position_sensor/."

cp -v "${LIBRARY_DIR}/SweepingServo.hpp" "${ARDUINOS_DIR}/servo_and_sensors/."

echo ""
echo "${SCRIPT_NAME} finished"
