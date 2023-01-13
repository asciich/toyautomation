
from Devices.DeviceBase import DeviceBase
from Devices.PpmRcActors.Train import Train


class PpmRcDevice(DeviceBase):

    N_CHANNELS = 8
    CHANNEL_VALUE_CENTER = 0

    def __init__(self, device_node):
        super(PpmRcDevice, self).__init__(device_node=device_node)
        self._actors = []

        self._channel_values = []
        for i in range(self.N_CHANNELS):
            self._channel_values.append(self.CHANNEL_VALUE_CENTER)

    def _send_center_all_channels(self):
        self.write_serial('c')

    def __del__(self):
        self._send_center_all_channels()

    def is_xjt_device(self):
        return True

    def define_train(self, channel_number: int, name: str = '') -> Train:
        train = Train(channel_number=channel_number, name=name)
        self._actors.append(train)
        return train

    def update_outputs(self):
        for actor in self._actors:
            channel_number = actor.get_channel_number()
            channel_value = actor.get_channel_value()
            self._channel_values[channel_number] = channel_value

        for i in range(self.N_CHANNELS):
            command = 's {} {}'.format(i, self._channel_values[i])
            self.write_serial(command)
