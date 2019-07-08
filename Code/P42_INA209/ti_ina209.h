// TI INA209 Arduino library
// created 20.03.2019
// Copyright (c) 2019 Wolfgnag Friedrich <wolfgangfriedrich42@gmail.com>.  All right reserved.
// Released under Creative Commons Attribution-Share Alike 4.0 International License

// Pier 42 Watt-A-Live Shield/Wing

// Last change: 2019/Jun/30

// https://www.tindie.com/stores/pier42/
// https://hackaday.io/project/166326-watt-a-live-power-monitor-shield-wing
// https://github.com/wolfgangfriedrich/P42-Watt-A-Live

#ifndef TI_INA209_H
#define TI_INA209_H

#include <Arduino.h>
#include <Wire.h> 		
 
 // Hardware - Select one of the 2 options !!!
 // Arduino Uno
//# define WARN_PIN  13
//# define ALERT_PIN 12
//# define GPIO_PIN  11
//# define CONV_PIN  10
//# define OVER_PIN   2
//# define CRIT_PIN   3

 // Feather
# define WARN_PIN  15
# define ALERT_PIN 14
# define GPIO_PIN  16
# define CONV_PIN  12
# define OVER_PIN   1
# define CRIT_PIN   0


						// Register Name		Function										power on reset bin	hex		type
#define CONFIG_REG				0x00	// Config Register		bus voltage range, PGA Gain, ADC				00111001 10011111	399F	R/W
#define STATUS_REG				0x01	// Status Register		warnings, limits, ready, overflow,alert			00000000 00000000	0000	R
#define SMBUS_REG				0x02	// SMBus Ctrl Register	En/disables flags in the Status Reg				00000000 00000000	0000	R/W
#define SHUNT_V_REG				0x03	// Shunt Voltage		Shunt voltage measurement data					00000000 00000000	0000	R
#define BUS_V_REG				0x04	// Bus Voltage			Bus voltage measurement data					00000000 00000000	0000	R
#define POWER_REG				0x05	// Power				Power measurement data							00000000 00000000	0000	R
#define CURRENT_REG				0x06	// Current				Value of the current through the shunt			00000000 00000000	0000	R
#define SHUNT_V_POSPEAK_REG		0x07	// Shunt V Pos Peak		Max pos V reading of Shunt V Reg				10000000 00000000	8000	R/W
#define SHUNT_V_NEGPEAK_REG		0x08	// Shunt V Neg Peak		Max neg V reading of Shunt V Reg				01111111 11111111	7FFF	R/W
#define BUS_V_MAX_REG			0x09	// Bus Voltage Max Peak	Highest V reading of Bus V Reg					00000000 00000000	0000	R/W
#define BUS_V_MIN_REG			0x0A	// Bus Voltage Min Peak	Lowest V reading of Bus V Reg					11111111 11111000	FFF8	R/W
#define POWER_PEAK_REG			0x0B	// Power Peak			Highest power reading of Power Reg				00000000 00000000	0000	R/W
#define SHUNT_V_POSWARN_REG		0x0C	// Shunt V Pos Warning	Set pos shunt V to trigger warning flag/pin		00000000 00000000	0000	R/W
#define SHUNT_V_NEGWARN_REG		0x0D	// Shunt V Neg Warning	Set neg shunt V to trigger warning flag/pin		00000000 00000000	0000	R/W
#define POWER_WARN_REG			0x0E	// Power Warning		Set Power to trigger warning flag/pin			00000000 00000000	0000	R/W
#define BUS_OVER_V_WARN_REG		0x0F	// Bus Over-V Warning	Set over Bus V to trigger warning flag/pin		00000000 00000000	0000	R/W
#define BUS_UNDER_V_WARN_REG	0x10	// Bus Under-V Warning	Set under Bus V to trigger warning flag/pin		00000000 00000000	0000	R/W
#define POWER_LIMIT_REG			0x11	// Power Over-Limit		Set power limit to trigger limit flag/pin		00000000 00000000	0000	R/W
#define BUS_OV_LIMIT_REG		0x12	// Bus Over-V Over-Lim	Set Bus over V limit to trig flag/pin-pol/latch	00000000 00000000	0000	R/W
#define BUS_UV_LIMIT_REG		0x13	// Bus Under-V Over-Lim	Set Bus under V limit to trigger flag/pin		00000000 00000000	0000	R/W
#define CRIT_DACP_REG			0x14	// Critical DAC+		Set DAC+ critical limit - crit config1			00000000 00000000	0000	R/W
#define CRIT_DACN_REG			0x15	// Critical DAC–		Set DAC- critical limit - crit config2			00000000 00000000	0000	R/W
#define CALIBRATION_REG			0x16	// Calibration			Set Range for curr/power - system calibration	00000000 00000000	0000	R/W


// Config reg 0x00
#define RST		0x8000		// Reset bit
#define BRNG	0x2000		// Bus voltage range
#define PGA		0x1800		// Shunt voltage mask
#define BADC	0x0780		// Bus ADC Resolution/Averaging
#define SADC	0x0078		// Shunt ADC Resolution/Averaging
#define MODE	0x0007		// Operating Mode

// Status reg 0x01
#define WOV		0x8000		// Warning Bus Over-Voltage
#define WUV		0x4000		// Warning Bus Under-Voltage
#define WP		0x2000		// Warning Power
#define WSP		0x1000		// Warning Shunt+ Voltage
#define WSN		0x0800		// Warning Shunt– Voltage
#define OLOV	0x0400		// Over-Limit Bus Over-Voltage
#define OLUV	0x0200		// Over-Limit Bus Under-Voltage
#define OLP		0x0100		// Over-Limit Power
#define CRITP	0x0080		// Critical Shunt Positive Voltage
#define CRITN	0x0040		// Critical Shunt Negative Voltage
#define CNVR	0x0020		// Conversion Ready
#define SMBA	0x0010		// SMBus Alert
#define OVF		0x0008		// Math Overflow

// SMBus Alert Mask/Enable Control reg 0x02
#define MWOV	0x8000		// Warning Bus Over-Voltage Mask
#define MWUV	0x4000		// Warning Bus Under-Voltage Mask
#define MWP		0x2000		// Warning Power Mask
#define MWSP	0x1000		// Warning Shunt+ Voltage Mask
#define MWSN	0x0800		// Warning Shunt– Voltage Mask
#define MOLOV	0x0400		// Over-Limit Bus Over-Voltage Mask
#define MOLUV	0x0200		// Over-Limit Bus Under-Voltage Mask
#define MOLP	0x0100		// Over-Limit Power Mask
#define MCRITP	0x0080		// Critical Shunt Positive Voltage Mask
#define MCRITN	0x0040		// Critical Shunt Negative Voltage Mask
#define MCNVR	0x0020		// Conversion Ready Mask
#define SMAEN	0x0008		// SMBus Alert Enable
#define CREN	0x0004		// Critical DAC Enable
#define OLEN	0x0002		// Over-Limit Enable
#define WRNEN	0x0001		// Warning Enable

// Bus Over-Voltage Warning reg 0x0F
#define BWO		0xFFF8		// Bus over-voltage warning limit
#define WPL		0x0002		// Warning pin polarity
#define WNL		0x0001		// Latching feature of the Warning pin

// Bus Over-Voltage Over-Limit reg 0x12
#define BOO		0xFFF8		// Bus over-voltage over-limit limit
#define OLP		0x0002		// over-limit pin polarity
#define OLL		0x0001		// Latching feature of the over-limit pin

// Critical DAC+ Register (Critical Shunt Positive Voltage) reg 0x14
#define CDP			0xFF00	// Critical DAC+ limit
#define GP			0x0080	// GPIO read back
#define GPM			0x0060	// GPIO mode
#define GPMIN		0x0000	// GPIO mode input
#define GPMOUT0		0x0040	// GPIO mode output 0
#define GPMOUT1		0x0060	// GPIO mode output 1


#define CP			0x0010	// Critical output pin polarity (open-drain output)
#define CHYST14MV	0x000E	// Critical comparator hysteresis 14mV
#define CHYST12MV	0x000C	// Critical comparator hysteresis 12mV
#define CHYST10MV	0x000A	// Critical comparator hysteresis 10mV
#define CHYST8MV	0x0008	// Critical comparator hysteresis 8mV
#define CHYST6MV	0x0006	// Critical comparator hysteresis 6mV
#define CHYST4MV	0x0004	// Critical comparator hysteresis 4mV
#define CHYST2MV	0x0002	// Critical comparator hysteresis 2mV
#define CHYST0MV	0x0000	// Critical comparator hysteresis 0mV
#define CRL			0x0001	// Critical pin latch feature

// Critical DAC– Register (Critical Shunt Negative Voltage) reg 0x15
#define CDN		0xFF00		// Mask Critical DAC- limit
#define MCF		0x00F0		// Mask DAC Comparator output filter
#define MWD		0x000F		// Mask Warning pin Output Delay from 0 to 1.5s; 0.1 second/LSB

#define CF000	0x0000		// Filter setting 0ms
#define CF064	0x0010		// Filter setting 0.064ms
#define CF128	0x0020		// Filter setting 0.128ms
#define CF192	0x0030		// Filter setting 0.192ms
#define CF256	0x0040		// Filter setting 0.256ms
#define CF320	0x0050		// Filter setting 0.320ms
#define CF384	0x0060		// Filter setting 0.384ms
#define CF448	0x0070		// Filter setting 0.448ms
#define CF512	0x0080		// Filter setting 0.512ms
#define CF576	0x0090		// Filter setting 0.576ms
#define CF640	0x00A0		// Filter setting 0.640ms
#define CF704	0x00B0		// Filter setting 0.704ms
#define CF768	0x00C0		// Filter setting 0.768ms
#define CF832	0x00D0		// Filter setting 0.832ms
#define CF896	0x00E0		// Filter setting 0.896ms
#define CF960	0x00F0		// Filter setting 0.960ms

#define WD00	0x0000		// Output Delay setting 0.0ms
#define WD01	0x0001		// Output Delay setting 0.1ms
#define WD02	0x0002		// Output Delay setting 0.2ms
#define WD03	0x0003		// Output Delay setting 0.3ms
#define WD04	0x0004		// Output Delay setting 0.4ms
#define WD05	0x0005		// Output Delay setting 0.5ms
#define WD06	0x0006		// Output Delay setting 0.6ms
#define WD07	0x0007		// Output Delay setting 0.7ms
#define WD08	0x0008		// Output Delay setting 0.8ms
#define WD09	0x0009		// Output Delay setting 0.9ms
#define WD10	0x000A		// Output Delay setting 1.0ms
#define WD11	0x000B		// Output Delay setting 1.1ms
#define WD12	0x000C		// Output Delay setting 1.2ms
#define WD13	0x000D		// Output Delay setting 1.3ms
#define WD14	0x000E		// Output Delay setting 1.4ms
#define WD15	0x000F		// Output Delay setting 1.5ms

// PGA 
#define PGA1	0x0000		// Shunt voltage gain = 1 range =  +-40mV
#define PGAdiv2	0x0800		// Shunt voltage gain = 2 range =  +-80mV
#define PGAdiv4	0x1000		// Shunt voltage gain = 4 range = +-160mV
#define PGAdiv8	0x1800		// Shunt voltage gain = 8 range = +-320mV

// BADC Bus ADC Resolution/Averaging
#define BADC9b		0x0000		// BADC mode 9-bit
#define BADC10b		0x0080		// BADC mode 10-bit
#define BADC11b		0x0100		// BADC mode 11-bit
#define BADC12b		0x0180		// BADC mode 12-bit
#define BADC12b_2	0x0400		// BADC mode 12-bit
#define BADC2		0x0480		// BADC averaging 2 samples
#define BADC4		0x0500		// BADC averaging 4 samples
#define BADC8		0x0580		// BADC averaging 8 samples
#define BADC16		0x0600		// BADC averaging 16 samples
#define BADC32		0x0680		// BADC averaging 32 samples
#define BADC64		0x0700		// BADC averaging 64 samples
#define BADC128		0x0780		// BADC averaging 128 samples

// SADC Shunt ADC Resolution/Averaging
#define SADC9b		0x0000		// SADC mode 9-bit
#define SADC10b		0x0008		// SADC mode 10-bit
#define SADC11b		0x0010		// SADC mode 11-bit
#define SADC12b		0x0018		// SADC mode 12-bit
#define SADC12b_2	0x0040		// SADC mode 12-bit
#define SADC2		0x0048		// SADC averaging 2 samples
#define SADC4		0x0050		// SADC averaging 4 samples
#define SADC8		0x0058		// SADC averaging 8 samples
#define SADC16		0x0060		// SADC averaging 16 samples
#define SADC32		0x0068		// SADC averaging 32 samples
#define SADC64		0x0070		// SADC averaging 64 samples
#define SADC128		0x0078		// SADC averaging 128 samples

// MODE Operating Mode
#define MODE_PWRDN		0x0000		// Power-Down
#define MODE_SV_TRIG	0x0001		// Shunt Voltage, Triggered
#define MODE_BV_TRIG	0x0002		// Bus Voltage, Triggered
#define MODE_SBV_TRIG	0x0003		// Shunt and Bus, Triggered
#define MODE_ADC_OFF	0x0004		// ADC Off (disabled)
#define MODE_SV_CONT	0x0005		// Shunt Voltage, Continuous
#define MODE_BV_CONT	0x0006		// Bus Voltage, Continuous
#define MODE_SBV_CONT	0x0007		// Shunt and Bus, Continuous


#define CALIB_VALUE		4096		// from calculation in datasheet


class TI_INA209
{
int ina209_addr;
int reg_addr;
float shunt_f;

//private: 
//	word readWord();
//	void writeWord(byte reg_addr , word data);

public:
	TI_INA209(byte address);
//	TI_INA209(byte address, float shunt);
	word	readWord (byte reg_addr );
	void	writeWord (byte reg_addr , word data);
	void	pinMode209 ( uint8_t mode);
	void	digitalWrite209 ( uint8_t value );
	uint8_t	digitalRead209 ( void );
//	word	getCurrent ( float shunt_f );
//	word	getVoltage ( void );
//	word	getPower ( float shunt_f );
		
};


#endif
