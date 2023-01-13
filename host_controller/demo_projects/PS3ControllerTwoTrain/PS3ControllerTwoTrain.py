#!/usr/bin/env python
import logging

import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
library_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), 'library')
assert os.path.isdir(library_dir)
sys.path.insert(0, library_dir)

from ToyAutomation import ToyAutomation
from Devices.PS3Controller.PS3Controller import PS3Controller

def main():
    logging.getLogger().setLevel(logging.DEBUG)

    toy_automation = ToyAutomation()
    toy_automation.collect_hardware()

    rc_transmitter = toy_automation.get_xjt_device()
    main_train = rc_transmitter.define_train(channel_number=0, name='retos_train')
    second_train = rc_transmitter.define_train(channel_number=1, name='second_train')

    ps3_controller = toy_automation.get_next_ps3_controller()
    ps3_controller.add_analog_value_changed_function(PS3Controller.ANALOG_RIGHT_VERTICAL,
                                                     lambda toy_automation: main_train.set_channel_value(
                                                         ps3_controller.get_right_stick_vertical_percent()))

    ps3_controller2 = toy_automation.get_next_ps3_controller()
    ps3_controller2.add_analog_value_changed_function(PS3Controller.ANALOG_RIGHT_VERTICAL,
                                                     lambda toy_automation: second_train.set_channel_value(
                                                         ps3_controller2.get_right_stick_vertical_percent()))

    toy_automation.add_key_pressed_function('Key.up', lambda toy_automation: main_train.toggle_drive_direction())
    toy_automation.add_key_pressed_function('Key.down', lambda toy_automation: main_train.set_stop())
    toy_automation.add_key_pressed_function('s', lambda toy_automation: main_train.set_stop())
    toy_automation.add_key_pressed_function('Key.right', lambda toy_automation: main_train.set_drive_forward())
    toy_automation.add_key_pressed_function('a', lambda toy_automation: main_train.set_drive_forward())
    toy_automation.add_key_pressed_function('Key.left', lambda toy_automation: main_train.set_drive_backward())
    toy_automation.add_key_pressed_function('w', lambda toy_automation: main_train.set_drive_backward())

    toy_automation.add_key_pressed_function('1', lambda toy_automation: second_train.set_drive_backward())
    toy_automation.add_key_pressed_function('2', lambda toy_automation: second_train.set_stop())
    toy_automation.add_key_pressed_function('3', lambda toy_automation: second_train.set_drive_forward())

    def run_function(toy_automation: ToyAutomation):
        pass

    toy_automation.set_run_function(run_function=run_function)
    toy_automation.run()


if __name__ == '__main__':
    main()
