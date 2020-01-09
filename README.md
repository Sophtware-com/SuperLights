# Super Lights
These lights are a great way to have fun at twilight on your paramotor, impress your friends, and fly legally during twilight (strobe pattern required). This set comes pre-programmed with lot of different patterns, and most of them can be adjusted to create infinte variety.

## Device Platforms
Below is the list of Arduino devices the code is written toward. A minium of an ATMega328 processor is required due to the size of the code. Anything bigger is a plus, but the LED library timings may need to be adjusted for faster processors.

* Arduino Uno R3
* Arduino Nano v3.x

## How Super Lights Work

Super Lights is designed with 2 buttons, 3 potentiometers, 1 switch, 1 RGB LED, a USB port, and an MR30 connector for the LED lights. 

The switch is used to power the Arduino and the LEDs via an DPST style switch. When turned off, the USB port can be used to charge the onboard battery. When off, and a USB cable is plugged in, the Arduino and the onboard charging chip will reveive power. A pin on the processor will indicate if the unit is in a charging state and the processor will handle LED display for the different charging states: standby and charged.

The USB port is used for programming and charging the onboard battery. The Arduino used may include the FTDI or CH340 USB to serial chip, but this should not be a concern for programming since the latest version of Mac and Windows include both drivers now.

The 2 buttons are used for both user programming of settings and navigating the patterns and groups of patterns. When in running mode, the group button moves between the groups of patterns (like flags or holiday lights), while the pattern button moves between the individual patterns in the group (like from the American flag to the Mexican flag). Both of these buttons have an alternate feature when held down for 2 seconds or more. Doing this resets either the groups or the patterns to the first one in the sequence.

The 3 potentiometers control the brightness, flashing rate, and color or size within particular patterns. The brightness is controlled globally and affects the LEDs immediately and this setting is not saved to internal EEPROM. The flash rate and color/size knob settings are stored per individual pattern and the values are saved when moving off the pattern or turning off the unit.

## LED Initialization Mode

To enter LED initialization mode, with the unit off, turn all 3 knobs counter clockwise until they stop, then hold both group and pattern buttons down, then turn the unit on. When the lights display, let go of both buttons. The unit will enter LED initialization mode allowing the user to set the number of LEDs on their light set, where the 12 o'clock position is, both the 9 and 3 o'clock positions, and the direction the LEDs are mounted - either clockwise or counter-clockwise. This will allow the unit to adjust patterns on the fly so they display correctly on the user's cage ring.

### Alternate Settings for Naviagtion

Alternate settings for navigating patterns can be set using a programming sequence at start up. This can be set based in the user's preferences.

1. **Normal Operation - Mode 1**

    #### Description
    ---
    In this mode the group button jumps from group to group in sequence looping to the beginning once the end is reached. Holding down the group button for 2 seconds will reset back to the first group. The pattern button navigates the patterns in sequence within the groups looping when the end is reached. Holding down the pattern button for 2 seconds resets back to the first pattern in the selected group.

    #### How to Set this Mode
    ---
    Turn the Brightness knob counter clockwise until it stops, then turn the flash rate and color/size knobs clockwise until they stop. hold down both the group and pattern buttons and turn the unit on. When the lights display, let go of both buttons. The mode has now been set.

1. **Continuous Pattern Button - Mode 2**

    #### Description
    ---
    In this mode the group button functions as in normal operation mode. The pattern button, though, will continue to the next group automatically without pushing the group button. Meaning the pattern button doesn't loop when reaching the end, but continues to the next group. Holding the pattern button for 2 seconds will reset to the first pattern in the first group.

    #### How to Set this Mode
    ---
    Turn the Brightness and flash rate knob counter clockwise until they stop, then turn the color/size knob clockwise until it stops. hold down both the group and pattern buttons and turn the unit on. When the lights display, let go of both buttons. The mode has now been set.

1. **Favorite Patterns Looping - Mode 3**

    #### Description
    ---
    In this mode the group button functions as in normal operation mode. The pattern button, though, will continue to the next group automatically without pushing the group button. Meaning the pattern button doesn't loop when reaching the end, but continues to the next group. Holding the pattern button for 2 seconds will reset to the first pattern in the first group.

    #### How to Set this Mode
    ---
    Turn the Brightness and flash rate knob counter clockwise until they stop, then turn the color/size knob clockwise until it stops. hold down both the group and pattern buttons and turn the unit on. When the lights display, let go of both 