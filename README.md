# Super Lights
These lights are a great way to have fun at twilight on your paramotor, impress your friends, and fly legally during twilight (with included strobe pattern). This set comes pre-programmed with lot of different patterns, and most of them can be adjusted to create infinte variety.

## Device Platforms
Below is the list of Arduino devices the code is written toward. A minium of an ATMega328 processor is required due to the size of the code. Anything bigger is a plus, but the LED library timings may need to be adjusted for faster processors.

* Arduino Uno R3
* Arduino Nano v3.x

## How Super Lights Work

Super Lights is designed with 2 buttons, 3 potentiometers, 1 switch, 1 RGB LED, a USB port, and two MR30PW-M connectors for the LED lights. 

The switch is used to power the Arduino and the LEDs via an DPST style switch. When turned off, the USB port can be used to charge the onboard battery. When off, and a USB cable is plugged in, the Arduino and the onboard charging chip will receive power. A pin on the processor will indicate if the unit is in a charging state and the processor will handle LED display for the different charging states: standby and charged.

The USB port is used for programming and charging the onboard battery. The Arduino used may include the FTDI or CH340 USB to serial chip, but this should not be a concern for programming since the latest version of Mac and Windows include both drivers now. Also, on the Nano, it doesn't matter if it's the old or new bootloaded. The doesn't use the Watch Dog Reset function.

The 2 buttons are used for both user programming of settings and navigating the patterns and groups of patterns. When in running mode, the group button moves between the groups of patterns (like flags or holiday lights), while the pattern button moves between the individual patterns in the group (like from the American flag to the Mexican flag). Both of these buttons have an alternate feature when held down for 2 seconds or more. Doing this resets either the groups or the patterns to the first one in the sequence. This can change based on the Mode the lights are set to via programming.

The 3 potentiometers control the brightness, flashing rate, and color or size within particular patterns. The brightness is controlled globally and affects the LEDs immediately and this setting is not saved to internal EEPROM. The flash rate and color/size knob settings are stored per individual pattern and the values are saved when moving off the pattern or turning off the unit. The brightness knob does not work on all patterns, especially patterns that vary the brightness already like strobes and some rainbow patterns.  

## LED Initialization

The controller already comes pre-programmed for a 4 meter strand (240 LEDs) that is mounted with the center of the strand at the top of the cage. If you need to mount the LEDs off-center, or shorten the strand, you will need to reprogram the set so the controller can adjust the patterns to match your ring. See below.

### Alternate Settings for Naviagtion

Alternate settings for navigating and displaying patterns can be set using a button/knob setting sequence at start up. Entering one of these modes only lasts while the controller is on. Once turned off and on again, normal mode will resume.

#### Available Modes

    NORMAL, INITIALIZE, RANDOM, FAVORITES, CYCLE_LONG, CYCLE_SHORT, CONTINUOUS, STROBES

1. **NORMAL (0) - Normal Operation**

    #### Description
    ---
    In this mode the group button jumps from group to group in sequence looping to the beginning once the end is reached. Holding down the group button for 2 seconds will reset back to the first group. The pattern button navigates the patterns in sequence within the groups looping when the end of the group is reached. Holding down the pattern button for 2 seconds resets back to the first pattern in the selected group.

    The last pattern displayed in each group, and it's knob settings, is saved as a favorite and used in the FAVORITES mode.

    When turning off the controller, the last group and pattern are saved and restored the next time the controller is turned on in NORMAL mode.

    #### How to Set this Mode
    ---
    - &#x2611; Turn the controller on without holding any buttons down and this mode is set automatically.

1. **INITIALIZE (1) - Initialize LEDs**

    #### Description
    ---
    To enter LED initialization mode, with the unit off, turn all 3 knobs counter clockwise until they stop, then hold both group and pattern buttons down, then turn the unit on. When the lights display, let go of both buttons. The unit will enter LED initialization mode allowing the user to set the number of LEDs on their light set (even numbers only), where the 12 o'clock position is, both the 9 and 3 o'clock positions, and the direction the LEDs are mounted - either clockwise or counter-clockwise. This will allow the unit to adjust patterns on the fly so they display correctly on the user's cage ring (like the American flag and the navigation strobe pattern).

    #### How to Set this Mode
    ---
    - &#x21ba; *Brightness* knob full counter clockwise
    - &#x21ba; *Flash Rate* knob full counter clockwise
    - &#x21ba; *Color/Size* knob full counter clockwise
    - &#x21a7; Hold down *Pattern* and *Group* and switch ON.
    - &#x2611; The ring will flash RED 3 times to indicate INITIALZE mode is set.

1. **RANDOM (2) - Cycle Random Patterns**

    #### Description
    ---
    In this mode the *Group* button acts as a toggle between cycling through random patterns in all groups, and entering the STROBE group. When the controller is displaying random patterns, the *Pattern* button will pause the cycling. Pushing the *Pattern* button again will resume the cycling. If in the STROBE group, the *Pattern* button will act as normal. Holding down either button for 2 seconds does nothing.

    #### How to Set this Mode
    ---
    - *Brightness* knob don't care
    - *Flash Rate* knob sets delay between patterns from 3 to 15 seconds. Set position **before** turning on.
    - *Color/Size* knob dont' care
    - &#x21a7; Hold down *Pattern* and switch ON.
    - &#x2611; The mode is set and briefly displays segments matching the number.

1. **FAVORITES (3) - Cycle Favorite Patterns**

    #### Description
    ---
    In this mode the lights will cycle through the last pattern selected in each group (except the strobe group). The pattern button will pause and un-pause the cycling. So if you would like to stop on one pattern you can press the pattern button to pause the cycle. Pushing the pattern button again will resume the cycling. The group button will toggle from cycling through patterns to entering the strobe patterns. Once in the strobe patterns, the pattern button selectes the strobe pattern displaying. Pressing the group button again toggles back to pattern cycling. Each pattern is displayed for 8 seconds in this mode.

    #### How to Set this Mode
    ---
    - &#x21bb; *Brightness* knob full clockwise
    - &#x21bb; *Flash Rate* knob full clockwise
    - &#x21ba; *Color/Size* knob full counter clockwise
    - &#x21a7; Hold down *Pattern* and *Group* and switch ON.
    - &#x2611; The mode is set and displayed as segments matching the number.

1. **Mode 4 - All Patterns Looping**

    #### Description
    ---
    This mode is the same as the *Favorite Pattern Looping* mode except ALL the patterns are looped through in each group in sequence, again skipping the strobe pattern group. 

    #### How to Set this Mode
    ---
    - &#x21bb; *Brightness* knob full clockwise
    - &#x21ba; *Flash Rate* knob full counter clockwise
    - &#x21bb; *Color/Size* knob full clockwise
    - &#x21a7; Hold down *Pattern* and *Group* and switch ON.
    - &#x2611; The mode is set and displayed as segments matching the number.

1. **Mode 5 - Aircraft Strobes Only**

    #### Description
    ---
    In this mode only the strobe patterns are available. The pattern button switches strobe patterns as normal, but the group button does nothing. 

    #### How to Set this Mode
    ---
    - &#x21ba; *Brightness* knob full counter clockwise
    - &#x21ba; *Flash Rate* knob full counter clockwise
    - &#x21ba; *Color/Size* knob full counter clockwise
    - &#x21a7; Hold down *Pattern* and *Group* and switch ON.
    - &#x2611; The mode is set and displayed as segments matching the number.

## Charging the Battery

In order to charge the internal battery, the unit must be switched off. Once switched off, connecting a USB cable to the unit will initiate charging. The power LED will display BLUE when charging, and GREEN when complete (or in standby mode if left plugged into a charger or the battery is already fully charged).

The internal 10,000mAh battery can take up to 10 hours to charge when supplied with a USB port capable of delivering 1 amp of power. If the USB port cannot supply the full 1 amp, charging such a large battery can take several days to a week to complete charging. Make sure to use only ports capable of suppling 1 amp of current. If available, the unit can use up to 1.2 amp to charge.