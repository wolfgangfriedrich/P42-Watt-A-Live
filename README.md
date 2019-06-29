# P42-Watt-A-Live
Pier42 Watt-A-Live Power Monitor

The Watt-A-Live is a versatile embedded power monitor based on the Texas Instruments INA209 with full connectivity to the unit under test. It measures the load current through a shunt resistor and has separate GND connections to the power supply and the load to make it a true power monitor. 
It is designed as an Adafruit Feather Wing and Arduino Shield. Alternatively it can be used as a breakout board with any other controller that has I2C and optional GPIO connectivity. 2 jumpers can select between 2 different shunts to measure different current ranges. 
The board is populated with a 0.10 Ohm shunt to measure 3.2A to 100µA. Optionally a 500 Ohm shunt can be added for a current range of 640uA to 20nA. Maximum bus voltage is 26V.

Full feature set:
•	Adafruit Feather Wing and Arduino Uno Shield connector option
•	Current monitor with 2 different ranges for high current and sleep mode measurements 
•	Positive and negative current flow
•	Bus voltage monitor with 2 dedicated GND terminals
•	Communication interface: I2C up to 3.4MHz
•	16 I2C addresses selectable through resistor options
•	6 dedicated signaling pins (SMBus Alert, Warning, Overlimit, Critical, Convert and 1 GPIO)
•	Operating supply voltage 3.0V to 5.5V
•	Optional 4mm Banana plugs for bypass 
•	Size: 71mm x 54mm (2.8" x 2.1")
