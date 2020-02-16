
class PpmRcActorsBase(object):

    def __init__(self, channel_number):
        self._channel_number = channel_number
        self._channel_value = 0

    def get_channel_number(self):
        return self._channel_number

    def get_channel_value(self):
        return self._channel_value

    def set_channel_value(self, new_value):
        self._channel_value = new_value