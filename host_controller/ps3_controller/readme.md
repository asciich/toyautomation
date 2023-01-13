# Gamepad / PlayStation 3 controller

## First time setup:

Install the needed tools on your raspberry pi:

```bash
RPI_IP=>YOUR_RASPBERRY_PI_IP> ./../install_on_rpi.sh
```

## Connect the controller

1. Connect controller using USB
1. Run `sudo /opt/sixpair/sixpair`
1. Run `sudo bluetoothctl`
    1. Use these commands:
    1. `agent on`
    1. `default-agent`
    1. `scan on`
    1. Disconnect controller.
    1. Connect controller again
    1. Press "P3" button on the controller
    1. Only one led should now blink and the device address should be shown in the terminal.
    1. Do authorize the service (`yes`).
    1. `scan off`
    1. `quit`
1. `sudo reboot`
1. Disconnect controller
1. After reboot completed press P3 button to connect.

## Verify connected:

1. Run `sudo python /opt/toyautomation/ps3_controller/verify_js0.py`
    1. Run `sudo python /opt/toyautomation/ps3_controller/verify_js1.py` for the second controller.
1. Move sticks around.
