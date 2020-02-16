import time

import os

from Devices.DeviceFactory import DeviceFactory


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

    def run(self):
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
