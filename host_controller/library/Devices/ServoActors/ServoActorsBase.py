
class ActorsBase(object):

    def __init__(self, servo_number):
        self._servo_number = servo_number
        self._servo_position = 90

    def get_servo_position(self):
        return self._servo_position

    def get_servo_number(self):
        return self._servo_number

    def set_servo_position(self, new_position):
        self._servo_position = int(new_position)