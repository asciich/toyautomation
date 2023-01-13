#!/usr/bin/env bash

# This script installs all needed tools to use ToyAutomation on a raspberry pi.

set -e

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd ${SCRIPT_DIR}
SCRIPT_NAME="$(basename -- "${BASH_SOURCE[0]}" )"
REPO_ROOT="$(git rev-parse --show-toplevel)"

if [ -z "${RPI_IP}" ] ; then
  echo "RPI_IP not set error!"
  exit 1
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
cd "${SCRIPT_DIR}"

/opt/ve_ansible/bin/ansible-playbook -i "${RPI_IP}," ansible/install_toyautomation_host_controller.yml
/opt/ve_ansible/bin/pytest --hosts="ssh://root@${RPI_IP}" "${SCRIPT_DIR}"/tests/verify*.py