#!/usr/bin/env bash

set -e 

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd ${SCRIPT_DIR}
SCRIPT_NAME="$(basename -- "${BASH_SOURCE[0]}" )"

ARDUINOS_DIR="$( dirname "${SCRIPT_DIR}" )"
LIBRARY_DIR="${ARDUINOS_DIR}/library"

echo ""
echo "copy library files"
${LIBRARY_DIR}/update_library_files_in_adruino_projects.sh

echo ""
echo "Compile and upload"

# arduino --board arduino:avr:uno --port /dev/ttyACM0 --upload marble_elevator.ino
arduino --board arduino:avr:uno --port /dev/ttyUSB0 --upload marble_elevator.ino

echo ""
echo "${SCRIPT_NAME} finished"
