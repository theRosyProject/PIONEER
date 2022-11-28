#ifndef __ADS1015_H__
#define __ADS1015_H__

#include "Arduino.h"

#include <Wire.h>

#define ADS1015_ADDRESS (0x48)

#define ADS1015_CONVERSIONDELAY (1) 
#define ADS1115_CONVERSIONDELAY (9)

#define ADS1015_REG_POINTER_MASK (0x03)      
#define ADS1015_REG_POINTER_CONVERT (0x00)   
#define ADS1015_REG_POINTER_CONFIG (0x01)    
#define ADS1015_REG_POINTER_LOWTHRESH (0x02) 
#define ADS1015_REG_POINTER_HITHRESH (0x03)  

#define ADS1015_REG_CONFIG_OS_MASK (0x8000)
#define ADS1015_REG_CONFIG_OS_SINGLE (0x8000)
#define ADS1015_REG_CONFIG_OS_BUSY (0x0000)
#define ADS1015_REG_CONFIG_OS_NOTBUSY (0x8000)
#define ADS1015_REG_CONFIG_MUX_MASK (0x7000)
#define ADS1015_REG_CONFIG_MUX_DIFF_0_1 (0x0000)
#define ADS1015_REG_CONFIG_MUX_DIFF_0_3 (0x1000)
#define ADS1015_REG_CONFIG_MUX_DIFF_1_3 (0x2000)
#define ADS1015_REG_CONFIG_MUX_DIFF_2_3 (0x3000)
#define ADS1015_REG_CONFIG_MUX_SINGLE_0 (0x4000)
#define ADS1015_REG_CONFIG_MUX_SINGLE_1 (0x5000)
#define ADS1015_REG_CONFIG_MUX_SINGLE_2 (0x6000)
#define ADS1015_REG_CONFIG_MUX_SINGLE_3 (0x7000)
#define ADS1015_REG_CONFIG_PGA_MASK (0x0E00)
#define ADS1015_REG_CONFIG_PGA_6_144V (0x0000)
#define ADS1015_REG_CONFIG_PGA_4_096V (0x0200)
#define ADS1015_REG_CONFIG_PGA_2_048V (0x0400)
#define ADS1015_REG_CONFIG_PGA_1_024V (0x0600)
#define ADS1015_REG_CONFIG_PGA_0_512V (0x0800)
#define ADS1015_REG_CONFIG_PGA_0_256V (0x0A00)
#define ADS1015_REG_CONFIG_MODE_MASK (0x0100)
#define ADS1015_REG_CONFIG_MODE_CONTIN (0x0000)
#define ADS1015_REG_CONFIG_MODE_SINGLE (0x0100)
#define ADS1015_REG_CONFIG_DR_MASK (0x00E0)
#define ADS1015_REG_CONFIG_DR_128SPS (0x0000)
#define ADS1015_REG_CONFIG_DR_250SPS (0x0020)
#define ADS1015_REG_CONFIG_DR_490SPS (0x0040)
#define ADS1015_REG_CONFIG_DR_920SPS (0x0060) 
#define ADS1015_REG_CONFIG_DR_1600SPS (0x0080) 
#define ADS1015_REG_CONFIG_DR_2400SPS (0x00A0) 
#define ADS1015_REG_CONFIG_DR_3300SPS (0x00C0) 
#define ADS1015_REG_CONFIG_CMODE_MASK (0x0010) 
#define ADS1015_REG_CONFIG_CMODE_TRAD (0x0000) 
#define ADS1015_REG_CONFIG_CMODE_WINDOW (0x0010) 
#define ADS1015_REG_CONFIG_CPOL_MASK (0x0008) 
#define ADS1015_REG_CONFIG_CPOL_ACTVLOW (0x0000) 
#define ADS1015_REG_CONFIG_CPOL_ACTVHI (0x0008) 
#define ADS1015_REG_CONFIG_CLAT_MASK (0x0004) 
#define ADS1015_REG_CONFIG_CLAT_NONLAT (0x0000) 
#define ADS1015_REG_CONFIG_CLAT_LATCH (0x0004) 
#define ADS1015_REG_CONFIG_CQUE_MASK (0x0003) 
#define ADS1015_REG_CONFIG_CQUE_1CONV (0x0000) 
#define ADS1015_REG_CONFIG_CQUE_2CONV (0x0001) 
#define ADS1015_REG_CONFIG_CQUE_4CONV (0x0002) 
#define ADS1015_REG_CONFIG_CQUE_NONE (0x0003) 

typedef enum
{
	GAIN_TWOTHIRDS = ADS1015_REG_CONFIG_PGA_6_144V,
	GAIN_ONE = ADS1015_REG_CONFIG_PGA_4_096V,
	GAIN_TWO = ADS1015_REG_CONFIG_PGA_2_048V,
	GAIN_FOUR = ADS1015_REG_CONFIG_PGA_1_024V,
	GAIN_EIGHT = ADS1015_REG_CONFIG_PGA_0_512V,
	GAIN_SIXTEEN = ADS1015_REG_CONFIG_PGA_0_256V
} adsGain_t;


class ROSY_ADS1115
{
private:
	uint8_t m_i2cAddress;
	uint8_t m_conversionDelay;
	uint8_t m_bitShift;
	adsGain_t m_gain;
	adsGain_t getGain(void);
public:
	ROSY_ADS1115(uint8_t i2cAddress = ADS1015_ADDRESS);
	bool begin(void);
	bool readADC_SingleEnded(uint8_t channel, uint16_t* _readVal = 0);
	void setGain(adsGain_t gain);
};

#endif
