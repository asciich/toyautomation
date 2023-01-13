from .verify_js0 import PS3Controller

import pytest

class TestPS3Controller(object):

    @pytest.mark.parametrize('analog_value, expected_percent', [
        (0, 0.0),
        (0.0, 0.0),
        (32767, 100.0),
        (32767./2.0, 50.0),
        (32767./4.0, 25.0),
        (-32767, -100.0),
        (-32767./2.0, -50.0),
        (-32767./4.0, -25.0),
    ])

    def test_map_analog_values_to_percent(self, analog_value, expected_percent):
        assert PS3Controller().map_analog_value_to_percent(analog_value) == expected_percent

