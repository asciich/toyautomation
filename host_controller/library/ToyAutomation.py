import time

import os

from Devices.DeviceFactory import DeviceFactory
from Devices.PS3Controller.PS3Controller import PS3Controller

import logging
from pynput import keyboard

class ServoDeviceNotFounderror(Exception):
    pass


class XjtDeviceNotFoundError(Exception):
    pass


class ToyAutomation(object):

    def __init__(self):
        self._device_factor = DeviceFactory();
        self._servo_device = None
        self._xjt_device = None
        self._loop_counter = 0
        self._key_press_actions = {}
        self._ps3_controllers = []

    def collect_hardware(self):
        for i in range(10):
            device_node = '/dev/ttyACM{}'.format(i)
            if os.path.exists(device_node):
                new_device = self._device_factor.get_device(device_node)

                if new_device.is_servo_device():
                    self._servo_device = new_device
                    continue

                if new_device.is_xjt_device():
                    self._xjt_device = new_device
                    continue

                else:
                    raise NotImplementedError()

    def get_next_ps3_controller(self) -> PS3Controller:
        next_ps3_controller_number = self.get_number_of_ps3_controllers()
        input_device_node = f'/dev/input/js{next_ps3_controller_number}'
        self._ps3_controllers.append(PS3Controller(
            input_device_node=input_device_node,
            toy_automation_reference=self
        ))
        logging.info(f'Getting next PS3Controller using input device node = "{input_device_node}"')
        return self._ps3_controllers[-1]

    def get_number_of_ps3_controllers(self) -> int:
        return len(self._ps3_controllers)

    def get_servo_device(self):
        if self._servo_device is None:
            self.collect_hardware()

        if self._servo_device is None:
            raise ServoDeviceNotFounderror()

        return self._servo_device

    def get_xjt_device(self):
        if self._xjt_device is None:
            self.collect_hardware()

        if self._xjt_device is None:
            raise XjtDeviceNotFoundError()

        return self._xjt_device

    def set_run_function(self, run_function):
        self._run_function = run_function

    def get_prefered_loop_time_sec(self):
        return 0.1

    def get_loop_counter(self):
        return self._loop_counter

    def add_key_pressed_function(self, key_name: str, key_function):
        if len(key_name) == 1:
            key_name = "'{key_name}'".format(key_name=key_name)
        self._key_press_actions[key_name] = key_function

    def run(self):
        logging.info('"{class_name}": run started'.format(class_name=self.__class__.__name__))

        def on_press(key):
            if str(key) in self._key_press_actions:
                self._key_press_actions[str(key)](self)
            else:
                print('No key action for "{}" implemented,'.format(key))

        listener = keyboard.Listener(
            on_press=on_press
        )
        listener.start()

        for ps3_controller in self._ps3_controllers:
            ps3_controller.start()

        while True:
            self._update_inputs()

            self._run_function(self)

            self._update_outputs()

            self._loop_counter += 1
            time.sleep(self.get_prefered_loop_time_sec())

    def _update_inputs(self):
        if self._servo_device is not None:
            self._servo_device.update_inputs()

    def _update_outputs(self):
        if self._servo_device is not None:
            self._servo_device.update_outputs()

        if self._xjt_device is not None:
            self._xjt_device.update_outputs()
