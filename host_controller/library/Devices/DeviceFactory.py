import os
import serial

from Devices.PpmRcDevice import PpmRcDevice
from Devices.ServoDevice import ServoDevice


class DeviceDoesNotExistsError(Exception):
    pass


class DeviceFactory(object):

    def __init__(self):
        pass

    def get_device(self, device_node):
        if not os.path.exists(device_node):
            raise DeviceDoesNotExistsError()

        device_type = self._get_device_type(device_node)

        if device_type == 'servo':
            return ServoDevice(device_node=device_node)
        elif device_type == 'ppm_rc':
            return PpmRcDevice(device_node=device_node)
        else:
            raise NotImplementedError()

    def _get_device_type(self, device_node):
        serial_connection = serial.Serial(device_node)
        try:
            serial_connection.write(b't\n')
            device_type = serial_connection.readline().decode().strip()
        finally:
            serial_connection.close()

        return device_type