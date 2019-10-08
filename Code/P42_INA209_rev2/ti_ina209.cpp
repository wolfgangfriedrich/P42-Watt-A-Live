// TI INA209 Arduino library
// created 20.03.2019
// Copyright (c) 2019 Wolfgnag Friedrich <wolfgangfriedrich42@gmail.com>.  All right reserved.
// Released under Creative Commons Attribution-Share Alike 4.0 International License

// Pier 42 Watt-A-Live Shield/Wing

// Last change: 2019/Oct/06

// https://www.tindie.com/stores/pier42/
// https://hackaday.io/project/166326-watt-a-live-power-monitor-shield-wing
// https://github.com/wolfgangfriedrich/P42-Watt-A-Live



#include "TI_INA209.h"
//#include <Arduino.h>



//<<constructor>> 
TI_INA209::TI_INA209(byte address, float shunt) {
	ina209_addr = address;
	shunt_f = shunt;
	
	pinMode (WARN_PIN,  INPUT);
	pinMode (ALERT_PIN, INPUT);
	pinMode (OVER_PIN,  INPUT);
	pinMode (CRIT_PIN,  INPUT);

}

// read a word from the register 
word TI_INA209::readWord( byte reg_addr) {	

byte MSB = 0;
byte LSB = 0;	

	Wire.beginTransmission(ina209_addr);      
	Wire.write(reg_addr);                  
	Wire.endTransmission();
	Wire.beginTransmission(ina209_addr);      
	Wire.requestFrom(ina209_addr, 2);    // read 2 bytes from register	
	MSB = Wire.read();    
	LSB = Wire.read();    
	Wire.endTransmission();
	return word(MSB,LSB);
}

// write a word into the register pointed
void TI_INA209::writeWord( byte reg_addr , word data) {	
	Wire.beginTransmission(ina209_addr);	
	Wire.write(reg_addr);
	Wire.write(highByte(data)); 
	Wire.write(lowByte(data));  
	Wire.endTransmission();
}

// set the GPIO pin on the INA209 to the selected mode
void TI_INA209::pinMode209 ( uint8_t mode ) {
	
	if (mode == INPUT) {
		writeWord ( CRIT_DACP_REG, readWord ( CRIT_DACP_REG ) | GPMIN  );
	}
	else if (mode == OUTPUT) {
		writeWord ( CRIT_DACP_REG, (readWord ( CRIT_DACP_REG ) & !GPM) | GPMOUT0  );
	}
	else 
		return;
	
	return;
}

// set the state of the GPIO pin on the INA209
void TI_INA209::digitalWrite209 ( uint8_t value ) {
	
	if (value == HIGH) {
		writeWord ( CRIT_DACP_REG, readWord ( CRIT_DACP_REG ) | GPMOUT1  );
		// Serial.print("<1>");
	}
	else if (value == LOW) {
		writeWord ( CRIT_DACP_REG, (readWord ( CRIT_DACP_REG ) & !GPM) | GPMOUT0  );
		// Serial.print("<0>");
	}
	else
		return;
	
	return;
}

// read the state of the GPIO pin on the INA209
uint8_t  TI_INA209::digitalRead209 ( void ) {
	
	if ( readWord ( CRIT_DACP_REG ) & GP == 0 ) {
		return LOW;
	}
	else {
		return HIGH;
	}
}

// return current value in human readable form
float	TI_INA209::getCurrent ( float shunt_f ) {

	uint16_t value;
	float return_value;
	
		value = readWord (CURRENT_REG );
		if ((value & 0x8000) == 0x0000) {
			if (shunt_f > 1)
				return_value = value * 0.01 / shunt_f * 1000 ;
			else
				return_value = value * 0.01 / shunt_f  ;
		}
		else {
			if (shunt_f > 1)
				return_value =  -( (value ^ 0xffff) +1  ) * 0.01 / shunt_f * 1000 ;
			else
				return_value =  -( (value ^ 0xffff) +1  ) * 0.01 / shunt_f  ;
		}

//		Serial.print(F(" Current Register        : "));
//		Serial.print( return_value );
//		Serial.print( "-");
//		Serial.println(F(" mA"));

	return return_value;
}

// return voltage value in human readable form
	float	TI_INA209::getVoltage ( void ) {

	float return_value;

		return_value = ( (readWord( BUS_V_REG ) >>3) *0.004 );

//		Serial.print(F(" Bus Voltage Register    : "));
//		Serial.print(return_value);
//		Serial.println(F(" V"));

	return return_value;
}

// return power value in human readable form
	float	TI_INA209::getPower ( float shunt_f ){

	float return_value;

		if (shunt_f > 1)
			return_value = readWord( POWER_REG ) * 0.0002 / shunt_f * 1000;
		else
			return_value = readWord( POWER_REG ) * 0.0002 / shunt_f;

//		Serial.print(F(" Power Register          : "));
//		Serial.print( return_value );
//		Serial.println(F(" W"));

	return return_value;
}






