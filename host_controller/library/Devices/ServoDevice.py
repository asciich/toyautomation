from Devices.DeviceBase import DeviceBase
from Devices.DigitalInputs.DigitalInputBase import DigitalInputBase
from Devices.ServoActors.Turnout import Turnout


class ServoDevice(DeviceBase):

    N_SENSOR_INPUTS = 6
    N_SERVO_OUTPUTS = 8
    SERVO_VALUE_CENTER = 90

    def __init__(self, device_node):
        super(ServoDevice, self).__init__(device_node=device_node)
        self._actors = []

        self._servo_values = []
        for i in range(self.N_SERVO_OUTPUTS):
            self._servo_values.append(self.SERVO_VALUE_CENTER)

        self._digital_inputs = []
        for i in range(self.N_SENSOR_INPUTS):
            self._digital_inputs.append(DigitalInputBase(input_number=i))

    def is_servo_device(self):
        return True

    def get_digital_sensor(self, input_number):
        return self._digital_inputs[input_number]

    def define_turnout(self, servo_number, straight_degrees, curve_degrees):
        turnout = Turnout(servo_number=servo_number, straight_degrees=straight_degrees, curve_degrees=curve_degrees)
        self._actors.append(turnout)
        return turnout

    def update_outputs(self):
        for actor in self._actors:
            servo_number = actor.get_servo_number()
            servo_position = actor.get_servo_position()
            self._servo_values[servo_number] = servo_position

        for i in range(self.N_SERVO_OUTPUTS):
            command = 's {} {}'.format(i, self._servo_values[i])
            self.write_serial(command)

    def update_inputs(self):
        self.write_serial('r')
        input_values = self.read_serial_line()
        input_values = '0x' + input_values
        input_values = int(input_values, 0)

        for i in range(self.N_SENSOR_INPUTS):
            new_value = input_values & (1 << i)
            if (new_value == 0):
                self._digital_inputs[i].update_value(new_value=False)
            else:
                self._digital_inputs[i].update_value(new_value=True)


    def print_sensor_values(self):
        print('=== Sensor values start')
        for i in range(len(self._digital_inputs)):
            print('Input {} = {}'.format(i, self._digital_inputs[i].get_value()))
        print('=== Sensor values end')
        print('')
