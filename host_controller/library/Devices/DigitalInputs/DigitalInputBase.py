
class DigitalInputBase(object):

    def __init__(self, input_number):
        self._input_number = input_number
        self._last_value = None
        self._current_value = None

    def update_value(self, new_value):
        self._last_value = self._current_value
        self._current_value = new_value

    def changed_to_on(self):
        if self.is_on():
            if self._last_value == False:
                return True

        return False

    def is_on(self):
        if self.get_value():
            return True
        else:
            return False

    def get_value(self):
        return self._current_value