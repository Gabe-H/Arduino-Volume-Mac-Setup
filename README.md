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

## Typical Usage
Rotate clockwise/counter-clockwise for volume increase/decrease, and (for macOS) press the encoder to switch to fine-tune volume

### Mac Setup Mode
Pressing and holding the encoder button will put the arduino in a 'setup mode', ready for the macOS keyboard setup screen. 
When it prompts to press the key right of LSHIFT, press the button, and for the button to the left of RSHIFT press the button again. <br>
Usage will continue as normal after.
