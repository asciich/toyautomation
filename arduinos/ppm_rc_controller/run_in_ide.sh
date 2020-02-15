#!/usr/bin/env bash

# This is a modified version of https://github.com/asciich/docker-arduino/blob/master/run_container.sh

set -e

MAIN_WD=$(pwd)
PROJECTS_DIR="ppm_rc_controller.ino"
ARDUINO_PROJECT_FILE=""
PROJECTS_MOUNTPOINT="/arduino_projects/"
VOLUMES=""

if [ "${PROJECTS_DIR}" == "" ] ; then
    echo "Usages:"
    echo "     ${0} ARDUINO_PROJECT_DIR"
    exit 1
fi

if ! [ -d ${PROJECTS_DIR} ] ; then
    if [ -f ${PROJECTS_DIR} ] && [[ "${PROJECTS_DIR}" == *.ino ]] ; then
        # A .ino project needs to be placed into a directory with the same name.
        ARDUINO_PROJECT_NAME=$(basename ${PROJECTS_DIR})
        ARDUINO_PROJECT_NAME=$(echo ${ARDUINO_PROJECT_NAME} | sed 's/.ino//g')
        ARDUINO_PROJECT_MOUNTPOINT="/${ARDUINO_PROJECT_NAME}/"
        ARDUINO_PROJECT_FILE=${ARDUINO_PROJECT_NAME}/${ARDUINO_PROJECT_NAME}.ino
        PROJECTS_DIR=$(dirname ${PROJECTS_DIR})
        PROJECTS_DIR=$(cd ${PROJECTS_DIR} && pwd)
        cd ${MAIN_WD}
        # Create an additional mount point with the same name as the .ino file
        VOLUMES="${VOLUMES} -v ${PROJECTS_DIR}:${ARDUINO_PROJECT_MOUNTPOINT}"
    else
        echo "Arduino projects directory does not exist:"
        echo "  \"${PROJECTS_DIR}\""
        exit 1
    fi
fi

PROJECTS_DIR=$(cd ${PROJECTS_DIR} && pwd)
cd ${MAIN_WD}
echo "Mount ${PROJECTS_DIR} as ${PROJECTS_MOUNTPOINT}"

echo ""
echo "Start container"
VOLUMES="${VOLUMES} -v ${PROJECTS_DIR}:${PROJECTS_MOUNTPOINT}"
VOLUMES="${VOLUMES} -v /tmp/.X11-unix:/tmp/.X11-unix"
VOLUMES="${VOLUMES} -v ${XAUTHORITY}:${XAUTHORITY}"
ENVIRONMENT="-e DISPLAY=${DISPLAY}"
ENVIRONMENT="${ENVIRONMENT} -e XAUTHORITY=${XAUTHORITY}"
CONTAINER=asciich/arduino

echo docker run --rm --net=host --privileged ${VOLUMES} ${ENVIRONMENT} -it ${CONTAINER} arduino ${ARDUINO_PROJECT_FILE}
docker run --rm --net=host --privileged ${VOLUMES} ${ENVIRONMENT} -it ${CONTAINER} arduino ${ARDUINO_PROJECT_FILE}
