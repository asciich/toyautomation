from Devices.PpmRcActors.PpmRcActorsBase import PpmRcActorsBase
import logging


class Train(PpmRcActorsBase):

    def __init__(self, channel_number: int, name: str):
        super(Train, self).__init__(channel_number=channel_number)
        self._name = name

    def ensure_driving(self):
        if self.get_channel_value() == 0:
            self.set_drive_forward()

    def get_name(self) -> str:
        return self._name

    def set_drive_forward(self):
        logging.info('Train "{}": Drive forward'.format(self.get_name()))
        self.set_channel_value(100)

    def set_drive_backward(self):
        logging.info('Train "{}": Drive backward'.format(self.get_name()))
        self.set_channel_value(-100)

    def set_stop(self):
        logging.info('Train "{}": Drive stop'.format(self.get_name()))
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
