from Devices.ServoActors.ServoActorsBase import ActorsBase


class Turnout(ActorsBase):

    def __init__(self, servo_number, straight_degrees, curve_degrees):
        super(Turnout, self).__init__(servo_number=servo_number)

        self._straight_degrees = straight_degrees
        self._curve_degrees = curve_degrees

        self.set_position_straight()

    def set_position_curve(self):
        self.set_servo_position(self._curve_degrees)

    def set_position_straight(self):
        self.set_servo_position(self._straight_degrees)

    def toggle(self):
        if self.get_servo_position() == self._straight_degrees:
            self.set_position_curve()
        else:
            self.set_position_straight()