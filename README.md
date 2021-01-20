# Arduino-Volume-Mac-Setup

Simple Arduino volume control with rotary encoder, that includes a way to setup on macOS.

## Wiring
Requires an Arduino Leonardo or Pro Micro
I use a rotary encoder modules, so I'm not aware of the raw pinouts. But from the module:
```
GND -> GND
+ -> 5v
SW -> 7
DT -> 6
CLK -> 5
```

## Usage
When you first power the arduino, it will be in a setup mode, where you can select what the button will do. <br>
Windows users will never need to change, but Mac users have the option.
<br>
<ul>
  <li> Windows: Turn the volume knob clockwise, and press the button.
    <br>From here on the button will toggle mute.
  </li>
  <li> macOS: Turn the volume knob counter-clockwise, and press the button. From here, it will ALWAYS be ready for keyboard setup, that macOS sometimes requires. When/if it prompts for the key to the right of the LSHIFT, press the button, and same for the next.
  <br>
  From here on the button will toggle fine-tune volume adjustment.
  </li>
</ul>
Your selection will be saved to EEPROM for future use.
<br>
<b>YOU CAN ALWAYS GO BACK TO THIS BY HOLDING THE BUTTON FOR 5 SECONDS DURING NORMAL OPERATION</b>
