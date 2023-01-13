import logging
import threading
import time
from typing import Union
import os
import struct



class PS3ControllerError(Exception):
    pass

class PS3Controller(threading.Thread):

    TYPE_BUTTON = 1
    TYPE_ANALOG = 2

    VALUE_BUTTON_PRESSED = 1
    VALUE_BUTTON_RELEASED = 0

    VALUE_ANALOG_MIN = -32767
    VALUE_ANALOG_MAX = 32767
    VALUE_ANALOG_CENTER = 0

    ANALOG_L2 = 2
    ANALOG_R2 = 5

    ANALOG_LEFT_HORIZONTAL = 0
    ANALOG_LEFT_VERTICAL = 1

    ANALOG_RIGHT_HORIZONTAL = 3
    ANALOG_RIGHT_VERTICAL = 4

    BUTTON_X = 0
    BUTTON_CIRCLE = 1
    BUTTON_Triangle = 2
    BUTTON_SQUARE = 3

    BUTTON_L1 = 4
    BUTTON_R1 = 5
    BUTTON_L2 = 6
    BUTTON_R2 = 7

    BUTTON_SELECT = 8
    BUTTON_START = 9
    BUTTON_P3 = 10

    BUTTON_UP = 13
    BUTTON_DOWN = 14
    BUTTON_LEFT = 15
    BUTTON_RIGHT = 16

    def __init__(self, input_device_node: str=None, toy_automation_reference=None):
        super().__init__()
        self._input_device_node = None
        self._update_input_thread = None
        self._stop_update_input_thread = False
        self._analog_values_changed_actions = {}
        self._toy_automation_reference = None

        self._right_stick_vertical_analog_value = self.VALUE_ANALOG_CENTER

        if input_device_node is not None:
            self.set_input_device_node(input_device_node=input_device_node)

        if toy_automation_reference is not None:
            self.set_toy_automation_reference(toy_automation_reference=toy_automation_reference)

    def add_analog_value_changed_function(self, analog_input_number: int, changed_function):
        if not isinstance(analog_input_number, int):
            raise TypeError(f'Expected analog_input_number of type "int" but got "{type(analog_input_number)}"')

        if analog_input_number != self.ANALOG_RIGHT_VERTICAL:
            raise NotImplementedError('Only analog right vertical implemented at the moment.')

        self._analog_values_changed_actions[analog_input_number] = changed_function

    def get_input_device_node(self) -> str:
        return self._input_device_node

    def get_right_stick_vertical_percent(self) -> float:
        return self.map_analog_value_to_percent(analog_value=self._right_stick_vertical_analog_value)

    def get_toy_automation_reference(self):
        if self._toy_automation_reference is None:
            raise ValueError('Toy automation reference not set!')

        return self._toy_automation_reference

    def map_analog_value_to_percent(self, analog_value: Union[float, int]):
        analog_value = float(analog_value)

        old_range = float(self.VALUE_ANALOG_MAX - self.VALUE_ANALOG_MIN)

        new_min = -100
        new_max = 100
        new_range = float(new_max - new_min)
        return (((analog_value - self.VALUE_ANALOG_MIN) * new_range) / old_range) + new_min

    def run(self):
        logging.info('Background thread to read PS controller input started.')
        self._stop_background_thread = False

        while not self._stop_background_thread:
            try:
                # Source for reading ps3 controller data https://stackoverflow.com/questions/16032982/getting-live-info-from-dev-input
                EVENT_SIZE = struct.calcsize("LhBB")
                file = open(self.get_input_device_node(), "rb")
                event = file.read(EVENT_SIZE)
                while event:
                    if self._stop_background_thread:
                        break

                    print(struct.unpack("LhBB", event))
                    (tv_msec, value, input_type, number) = struct.unpack("LhBB", event)

                    if input_type == self.TYPE_ANALOG:
                        if number == self.ANALOG_RIGHT_VERTICAL:
                            self._right_stick_vertical_analog_value = value * -1

                        if number in self._analog_values_changed_actions:
                            self._analog_values_changed_actions[number](self.get_toy_automation_reference())

                    event = file.read(EVENT_SIZE)
            except (FileNotFoundError, OSError):
                RETRY_IN_SECONDS = 3
                logging.error(f'No PS3Controller {self.get_input_device_node()} found. Going to retry in "{RETRY_IN_SECONDS}".')
                time.sleep(RETRY_IN_SECONDS)

        logging.info('Background thread to read PS controller input finished.')

    def set_input_device_node(self, input_device_node:str ) -> None:
        if not isinstance(input_device_node, str):
            raise ValueError(f'Expected input_device_node of type "str" but got "{type(input_device_node)}".')

        if not os.path.isabs(input_device_node):
            raise ValueError(f'Expected input_device_node as absolute path but go "{input_device_node}".')

        self._input_device_node = input_device_node

    def set_toy_automation_reference(self, toy_automation_reference):
        from ToyAutomation import ToyAutomation
        if not isinstance(toy_automation_reference, ToyAutomation):
            raise ValueError(f'Expected toy_automation_reference of type "ToyAutomation" but got "{type(toy_automation_reference)}"')

        self._toy_automation_reference = toy_automation_reference

    def stop(self) -> None:
        self._stop_background_thread = True
        super().join()
