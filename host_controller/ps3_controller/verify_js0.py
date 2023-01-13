#!/usr/bin/env python

import time
import logging

from ..library.Devices.PS3Controller.PS3Controller import PS3Controller

if __name__ == '__main__':
    logging.getLogger().setLevel(logging.INFO)

    ps3_controller = PS3Controller(input_device_node='/dev/input/js0')
    ps3_controller.start()

    for i in range(10):
        print(f'right stick vertical percent {ps3_controller.get_right_stick_vertical_percent()}')
        time.sleep(1)

    ps3_controller.stop()
