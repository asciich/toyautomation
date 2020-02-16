#!/usr/bin/env python
import sys

import os

main_dir = os.path.dirname(os.path.abspath(__file__))
demo_project_dir = os.path.dirname(main_dir)
host_controller_dir = os.path.dirname(demo_project_dir)
library_dir = os.path.join(host_controller_dir, 'library')
assert os.path.exists(library_dir)
sys.path.insert(0, library_dir)

from ToyAutomation import ToyAutomation


def main():
    toy_automation = ToyAutomation()
    toy_automation.collect_hardware()

    servo_and_sensors = toy_automation.get_servo_device()

    main_turnout = servo_and_sensors.define_turnout(servo_number=0, straight_degrees=62, curve_degrees=100)
    main_turnout.set_position_straight()

    straight_end_sensor = servo_and_sensors.get_digital_sensor(input_number=0)
    curve_end_sensor = servo_and_sensors.get_digital_sensor(input_number=1)
    single_end_sensor = servo_and_sensors.get_digital_sensor(input_number=2)

    rc_transmitter = toy_automation.get_xjt_device()
    main_train = rc_transmitter.define_train(channel_number=0)


    def run_function(toy_automation):
        main_train.ensure_driving()

        if toy_automation.get_loop_counter() % 10 == 0:
            servo_and_sensors.print_sensor_values()

        if single_end_sensor.changed_to_on():
            main_turnout.toggle()
            main_train.toggle_drive_direction()

        if straight_end_sensor.changed_to_on():
            main_train.toggle_drive_direction()
            main_turnout.set_position_straight()
            pass

        if curve_end_sensor.changed_to_on():
            main_train.toggle_drive_direction()
            main_turnout.set_position_curve()

    toy_automation.set_run_function(run_function=run_function)
    toy_automation.run()




if __name__ == '__main__':
    main()
