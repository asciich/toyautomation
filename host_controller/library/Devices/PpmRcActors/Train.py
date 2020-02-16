from Devices.PpmRcActors.PpmRcActorsBase import PpmRcActorsBase


class Train(PpmRcActorsBase):

    def __init__(self, channel_number):
        super(Train, self).__init__(channel_number=channel_number)

    def ensure_driving(self):
        if self.get_channel_value() == 0:
            self.set_drive_forward()

    def set_drive_forward(self):
        self.set_channel_value(100)

    def set_drive_backward(self):
        self.set_channel_value(-100)

    def set_stop(self):
        self.set_channel_value(0)

    def is_driving_forward(self):
        if self.get_channel_value() > 0:
            return True
        return False

    def is_driving_backward(self):
        if self.get_channel_value() < 0:
            return True
        return False

    def is_stopped(self):
        if self.get_channel_value() == 0:
            return True
        return False

    def toggle_drive_direction(self):
        if self.is_driving_forward():
            self.set_drive_backward()
        else:
            self.set_drive_forward()
