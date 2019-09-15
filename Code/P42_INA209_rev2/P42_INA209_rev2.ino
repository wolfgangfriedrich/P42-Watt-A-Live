// TI INA209 Arduino Demo sketch
// created 20.03.2019
// Copyright (c) 2019 Wolfgnag Friedrich <wolfgangfriedrich42@gmail.com>.  All right reserved.
// Released under Creative Commons Attribution-Share Alike 4.0 International License

// Pier 42 Watt-A-Live Shield/Wing

// Last change: 2019/Sept/15

// https://www.tindie.com/stores/pier42/
// https://hackaday.io/project/166326-watt-a-live-power-monitor-shield-wing
// https://github.com/wolfgangfriedrich/P42-Watt-A-Live


#include <Wire.h>		// I2C library 
//#include <Serial.h>		// UART library 
#include "TI_INA209.h"

TI_INA209 ina209_40( 0x40, SHUNT_R );		// instantiate ina209_40 of class INA209 with I2C address 0x40. Address depends on set resistors.
										// Set shunt resistor value.

unsigned long ActionTime = 10000;
//word smbus_mask = MWOV | MWUV | MWP | MWSP | MWSN | MCRITP | MCRITN ;										// Set no bits for the warning flag 
word smbus_mask = MWOV | MWUV | MWP | MWSP | MWSN | MCRITP | MCRITN | SMAEN | OLEN | CREN | WRNEN;		// Set all bits for the warning flag 
//word smbus_mask = MWOV | MWUV | MWP | MWSP | MWSN | MCRITP | MCRITN | WRNEN;			// Warning 		
//word smbus_mask = MWOV | MWUV | MWP | MWSP | MWSN | MCRITP | MCRITN | OLEN;			// Overlimit only
//word smbus_mask = MWOV | MWUV | MWP | MWSP | MWSN | MCRITP | MCRITN | CREN;			// Critical only 
//word smbus_mask = MWOV | MWUV | MWP | MWSP | MWSN | MCRITP | MCRITN | SMAEN;			// SMBus Alert only 

word smbus_alert, last_smbus_alert;

byte warn_pin_status = 0;
byte alert_pin_status = 0;
byte over_pin_status = 0;
byte critical_pin_status = 0;
byte gpio_pin_status = 0;

byte toggle = 0;


void setup() {

word testword = 0;
	
	Wire.begin();
	Serial.begin(115200);
	
	delay(5000);
	
//	pinMode (WARN_PIN, INPUT);
	
//	testword = BRNG | (PGA & PGAdiv8) | (BADC & BADC12b)| (SADC & SADC12b) | MODE_SBV_CONT;
//	Serial.print(F(" Test word : 0x"));
//	Serial.println(testword,HEX );
	
	ina209_40.writeWord ( CONFIG_REG , BRNG | (PGA & PGAdiv8) | (BADC & BADC12b)| (SADC & SADC12b) | MODE_SBV_CONT );
	
//	Serial.print(F(" Configuration Register : 0x"));
//	Serial.println(ina209_40.readWord( CONFIG_REG ),HEX );

	ina209_40.writeWord ( CALIBRATION_REG , CALIB_VALUE );

//	Serial.print(F(" Calibration Register : 0x"));
//	Serial.println(ina209_40.readWord( CALIBRATION_REG ),HEX );

	// Set the warning/limit registers and configure the warning pin
	ina209_40.writeWord ( SHUNT_V_POSWARN_REG , 3000 );	// full-scale range = ±320mV, 15-bit + sign, LSB = 10µV
	ina209_40.writeWord ( SHUNT_V_NEGWARN_REG , 1000 );	// full-scale range = ±320mV, 15-bit + sign, LSB = 10µV
	ina209_40.writeWord ( POWER_WARN_REG , 6000 );			// Full-scale range and LSB are set by the Calibration Reg. Power = (Current * BusVoltage)/5000
	ina209_40.writeWord ( BUS_OVER_V_WARN_REG , 16000 | !WPL | !WNL );	// in 4mV LSB shifted 3 bits up (8V => 8000/4*8 = 16000) + warn pin polarity and latch 
	ina209_40.writeWord ( BUS_UNDER_V_WARN_REG , 7000 );	// in 4mV LSB shifted 3 bits up (3.5V => 3500/4*8 = 7000)

	ina209_40.writeWord ( BUS_OV_LIMIT_REG, 18000 | !OLP | !OLL );		// 9V -> set 9000*2 = 18000 + over pin polarity and latch
	ina209_40.writeWord ( BUS_UV_LIMIT_REG, 4000 );		// 2V -> set 2000*2 = 4000
	ina209_40.writeWord ( POWER_LIMIT_REG, 10000 );		// 

	ina209_40.writeWord ( CRIT_DACP_REG, 10240 | !CP | CHYST14MV | !CRL  );		// Full scale is 255mV, LSB = 1mV, upper 8 bit, 40mV : 40<<8 = 10240
	ina209_40.writeWord ( CRIT_DACN_REG, 10240 | CF000 | WD00 );		// no sign, DAC comparator output filter, delay for warning pin

	ina209_40.writeWord ( SMBUS_REG , smbus_mask );

	Serial.println(F(" TI INA209 Demo\r\n"));
	Serial.print(F(" Configuration Register : 0x"));
	Serial.println(ina209_40.readWord( CONFIG_REG ),HEX );
	delay(100);
	Serial.print(F(" Calibration Register : 0x"));
	Serial.println(ina209_40.readWord( CALIBRATION_REG ),HEX );

	// reset and clear peak-hold registers by writing 1 into D0 bit of the respective register
	ina209_40.writeWord ( SHUNT_V_POSPEAK_REG, 0x0001 );
	ina209_40.writeWord ( SHUNT_V_NEGPEAK_REG, 0x0001 );
	ina209_40.writeWord ( BUS_V_MAX_REG, 0x0001 );
	ina209_40.writeWord ( BUS_V_MIN_REG, 0x0001 );
	ina209_40.writeWord ( POWER_PEAK_REG, 0x0001 );
	
	// set warning watchdog registers	
	// values above these settings trigger flags in the status register and activate the warning pin.
	Serial.print(F(" SMBUS Alert Register : 0x"));
	Serial.println(ina209_40.readWord( SMBUS_REG ),HEX );
	
	ina209_40.pinMode209 ( OUTPUT );

	//Serial.print(F(" Critical DAC+ Register : 0x"));
	//Serial.println(ina209_40.readWord( CRIT_DACP_REG ),HEX );
	

	Serial.println("");

}


void loop() {

const long INTERVAL1000MSEC = 1000;
const long INTERVAL10SEC = 10000;
byte incomingByte = 0;
uint16_t value = 0;

unsigned long CurrentTime = millis();

//Serial.print(CurrentTime );
//Serial.print(" ");
//Serial.println(ActionTime);


	CurrentTime = millis();

//	if ( ( (CurrentTime - ActionTime) >= INTERVAL10SEC) || (Serial.available() != 0) )
	if ( (  Serial.available() != 0) )

	{

		if (Serial.available() != 0) 
			incomingByte = Serial.read();

		ActionTime = CurrentTime;
		
		if (toggle == 0) {
			toggle = 1;
			ina209_40.digitalWrite209 ( HIGH );
		}
		else {
			toggle = 0;
			ina209_40.digitalWrite209 ( LOW );
		}

		Serial.println("");
		Serial.print(F(" Bus Voltage Register    : "));
		Serial.print( (ina209_40.readWord( BUS_V_REG ) >>3) *0.004 );
		Serial.println(F(" V"));

	Serial.print(F("^^^ Bus Voltage Register : "));
	Serial.print( ina209_40.getVoltage() );
	Serial.println(F(" V"));

		Serial.print(F(" Current Register        : "));
		value = ina209_40.readWord (CURRENT_REG );
		if ((value & 0x8000) == 0x0000) {
			Serial.print( ( value ) * 0.01 / SHUNT_R );
//			Serial.print( ( value ) *0.2 );
		}
		else {
			Serial.print( "-");
			Serial.print( ( (value ^ 0xffff) +1  ) * 0.01 / SHUNT_R  );
		}
		Serial.println(F(" mA"));

	Serial.print(F("^^^ Current Register     : "));
	Serial.print( ina209_40.getCurrent( SHUNT_R ) );
	Serial.println(F(" mA"));

		Serial.print(F(" Shunt Voltage Register  : "));
		value = ina209_40.readWord (SHUNT_V_REG );
		if ((value & 0x8000) == 0x0000) {
			Serial.print( ( value ) * 0.01 );
		}
		else {
			Serial.print( "-");
			Serial.print( ( (value ^ 0xffff) +1  ) * 0.01 );
		}
		Serial.println(F(" mV"));
			
		Serial.print(F(" Power Register          : "));
		Serial.print(ina209_40.readWord( POWER_REG ) * 0.0002 / SHUNT_R );
		Serial.println(F(" W"));

	Serial.print(F("^^^ Power Register       : "));
	Serial.print(ina209_40.getPower( SHUNT_R ) );
	Serial.println(F(" W"));

		Serial.print(F(" Shunt Voltage Max Value : "));
		Serial.print( (ina209_40.readWord (SHUNT_V_POSPEAK_REG ) ) *0.01 );
		Serial.println(F(" mV"));

		Serial.print(F(" Shunt Voltage Min Value : "));
		value = ina209_40.readWord (SHUNT_V_NEGPEAK_REG );
		if ((value & 0x8000) == 0x0000) {
			Serial.print( ( value ) *0.01 );
		}
		else {
			Serial.print( "-" );
			Serial.print( ( (value ^ 0xffff) +1  ) *0.01 );
		}
			Serial.println(F(" mV"));
			

		Serial.print(F(" Bus Voltage Max Value   : "));
		Serial.print(ina209_40.readWord( BUS_V_MAX_REG ) >>1 );
		Serial.println(F(" mV"));

		Serial.print(F(" Bus Voltage Min Value   : "));
		Serial.print(ina209_40.readWord( BUS_V_MIN_REG ) >>1 );
		Serial.println(F(" mV"));

		Serial.print(F(" Power Max Value         : "));
		Serial.print(ina209_40.readWord( POWER_PEAK_REG ) *20 *SHUNT_R );
		Serial.println(F(" mW"));
		Serial.println("");
		
	}
	else {
		
		// Warn register functions
		smbus_alert = ina209_40.readWord ( STATUS_REG ) & smbus_mask;
		
		if (smbus_alert != last_smbus_alert ) {
			
			last_smbus_alert = smbus_alert;
			
//			Serial.print(F(" Status Register : 0x"));
//			Serial.println( smbus_alert, HEX );
			
			if ( smbus_alert & WOV) {				// Warning bus positive
				Serial.println (F(" !Warning Bus Voltage over"));
			}
			else if ( smbus_alert & WUV) {				// Warning bus negative
				Serial.println (F(" !Warning Bus Voltage under"));
			}
			else if ( smbus_alert & WP) {				// Warning power
				Serial.println (F(" !Warning Power"));
			}
			else if ( smbus_alert & WSP) {				// Warning shunt positive
				Serial.println (F(" !Warning positive Shunt Voltage"));
			}
			else if ( smbus_alert & WSN) {				// Warning shunt negative
				Serial.println (F(" !Warning negative Shunt Voltage"));
			} 
			else if ( smbus_alert & OLOV) {				// Overlimit over voltage
				Serial.println (F(" !!Limit Over Voltage"));
			} 
			else if ( smbus_alert & OLUV) {				// Overlimit under voltage
				Serial.println (F(" !!Limit Under Voltage"));
			} 
			else if ( smbus_alert & OLP) {				// Overlimit power
				Serial.println (F(" !!Limit Power"));
			} 
			else if ( smbus_alert & CRITP) {			// Critical Shunt Positive Voltage
				Serial.println (F(" !!!Critical DAC+"));
			} 
			else if ( smbus_alert & CRITN) {			// Critical Shunt Negative Voltage
				Serial.println (F(" !!!Critical DAC-"));
			} 


			else if ( smbus_alert == 0 ) {				// All warning bits clear
				Serial.println (F(" -No Warning - All Clear."));
			} 
			else  {				// Other cases.
				Serial.print (F(" !Warning unexpected value 0x"));
				Serial.println( smbus_alert, HEX );
			} 
		}
		
		delay (10);
		
	}
	
	// All pins are active low.
	// Warn pin
	if (digitalRead (WARN_PIN) && (warn_pin_status == 0)) {
		warn_pin_status = 1;
		Serial.println (F(" >Warning pin clear"));
	}
	else if (digitalRead (WARN_PIN))
		warn_pin_status = 1;
	else if (!digitalRead (WARN_PIN) && (warn_pin_status == 1)) {
		warn_pin_status = 0;
		Serial.println (F(" >Warning pin set"));		
	}
	else
		warn_pin_status = 0;
	
	// Alert pin
	if (digitalRead (ALERT_PIN) && (alert_pin_status == 0)) {
		alert_pin_status = 1;
		Serial.println (F(" >Alert pin clear"));
	}
	else if (digitalRead (ALERT_PIN))
		alert_pin_status = 1;
	else if (!digitalRead (ALERT_PIN) && (alert_pin_status == 1)) {
		alert_pin_status = 0;
		Serial.println (F(" >Alert pin set"));		
	}
	else
		alert_pin_status = 0;
	
	// Overlimit pin
	if (digitalRead (OVER_PIN) && (over_pin_status == 0)) {
		over_pin_status = 1;
		Serial.println (F(" >>Overlimit pin clear"));
	}
	else if (digitalRead (OVER_PIN))
		over_pin_status = 1;
	else if (!digitalRead (OVER_PIN) && (over_pin_status == 1)) {
		over_pin_status = 0;
		Serial.println (F(" >>Overlimit pin set"));		
	}
	else
		over_pin_status = 0;

	// Critical pin
	if (digitalRead (CRIT_PIN) && (critical_pin_status == 0)) {
		critical_pin_status = 1;
		Serial.println (F(" >>>Critical pin clear"));
	}
	else if (digitalRead (CRIT_PIN))
		critical_pin_status = 1;
	else if (!digitalRead (CRIT_PIN) && (critical_pin_status == 1)) {
		critical_pin_status = 0;
		Serial.println (F(" >>>Critical pin set"));		
	}
	else
		critical_pin_status = 0;


	// GPIO pin
	if (((ina209_40.readWord ( CRIT_DACP_REG ) & GP) == GP) && (gpio_pin_status == 0)) {
		gpio_pin_status = 1;
		Serial.println (F(" >>>GPIO pin set"));
	}
	else if (((ina209_40.readWord ( CRIT_DACP_REG ) & GP) == 0) && (gpio_pin_status == 1)) {
		gpio_pin_status = 0;
		Serial.println (F(" >>>GPIO pin clear"));		
	}
	else
		gpio_pin_status = gpio_pin_status;




}