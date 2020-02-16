import serial


class DeviceBase(object):

    def __init__(self, device_node):
        self._device_node = device_node
        self._serial_device = serial.Serial(device_node)

    def is_servo_device(self):
        return False

    def is_xjt_device(self):
        return False

    def update_inputs(self):
        raise NotImplementedError() # implement in derived objects

    def update_outputs(self):
        raise NotImplementedError() # implement in derived objects

    def write_serial(self, data):
        if not isinstance(data, str):
            raise NotImplementedError()

        if not data.endswith('\n'):
            data += '\n'

        data = data.encode()
        self._serial_device.write(data)


    def read_serial_line(self):
        line = self._serial_device.readline().decode().strip()
        return line
