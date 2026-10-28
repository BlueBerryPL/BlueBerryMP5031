#ifndef SimpleMP5031
#define SimpleMP5031
#include <Arduino.h>

// I2C standard address for MP5031 

#define STD_ADR 0x28

// Pointer register addresses used for accessing different registers 

#define PDO_TYPE 0x00  // Enables the chip 
#define PDO_V1 0x01  // Max current setting
#define PDO_I1 0x02  // Output setting
#define PDO_V2 0x02  // Max current setting
#define PDO_I2 0x03  // Output setting
#define PDO_V3 0x04  // Max current setting
#define PDO_I3 0x05  // Output setting
#define PDO_V4 0x06  // Max current setting
#define PDO_I4 0x07  // Output setting
#define PDO_V5 0x08  // Max current setting
#define PDO_I5 0x09  // Output setting
#define CTL1 0x0B  // Output setting
#define CTL2 0x0C  // Output setting
#define CTL3 0x0D  // Output setting
#define CTL4 0x0E  // Output setting
#define PWRSHA 0x0F  // Output setting
#define STATUS1 0x10  // Output setting
#define STATUS2 0x11  // Output setting
#define ID 0x12  // Output setting
#define CLK_ON 0x14  // Output setting

// User words

#define DISABLE 0
#define ENABLE 1

#define NONE 0
#define ALL 1

#define FPDO 0 // For PDO_TYPE
#define APDO 1 // For PDO_TYPE

#define PDO1 1 // For PDO_TYPE
#define PDO2 2 // For PDO_TYPE
#define PDO3 3 // For PDO_TYPE
#define PDO4 4 // For PDO_TYPE
#define PDO5 5 // For PDO_TYPE

#define PWR_SHARE_LIMT 2 // For VBATT_LOW_PULL

#define DCP 0 // For LEGACY_CHARGING
#define DIVIDER 2 // For LEGACY_CHARGING

#define MP298X 0 // For SLAVE_DEVICE_SEL
#define MP424X 1 // For SLAVE_DEVICE_SEL
#define MP28167 2 // For SLAVE_DEVICE_SEL
#define MP42550 3 // For SLAVE_DEVICE_SEL
#define MP42551 4 // For SLAVE_DEVICE_SEL
#define DIFF 5 // For SLAVE_DEVICE_SEL

#define LPS 1 // For LPS
#define PS1 2 // For LPS
#define PS2 3 // For LPS

#define EPS 1 // For TOUCH_CURRENT
#define GP 2 // For TOUCH_CURRENT
#define GPE 3 // For TOUCH_CURRENT

#define TS0 1 // For TOUCH_TEMP
#define TS1 2 // For TOUCH_TEMP
#define TS2 3 // For TOUCH_TEMP

#define ONCE 0 // For SRC_CAP
#define TWICE 1 // For SRC_CAP

#define PFM 0 // For PFM_PWM (Only when using MP28167 / MP298x)
#define PWM 1 // For PFM_PWM (Only when using MP28167 / MP298x)

#define EN 1 // For GPIO4 / GPIO3
#define VBATT 3 // For GPIO4 / GPIO2

#define ARB 0 // For GPIO3
#define ATTACH 2 // For GPIO3
#define EN_OFF 3 // For GPIO3
#define POL 4 // For GPIO3 / GPIO2
#define PDO3_SEL 5 // For GPIO3
#define ISENSP 6 // For GPIO3
#define IPWM 7 // For GPIO3

#define QC 0 // For GPIO2 / GPIO2
#define PULLDOWN 1 // For GPIO2
#define SHARE 2 // For GPIO2

#define PDO2_SEL 0 // For GPIO1
#define INT 2 // For GPIO1
#define OD 3 // For GPIO1
#define VSEL1 4 // For GPIO1

#define REDUCE 0 // For NTC_MODE
#define SHUTDOWN 1 // For NTC_MODE

#define VDRV0 0 // For VDRV
#define VDRV1 1 // For VDRV

#define DISCHG 0 // For GPIO7
#define EN_OUT_MID 1 // For GPIO7
#define EN_OUT 3 // For GPIO7
#define COMP 4 // For GPIO7

#define PDO4_SEL 0 // For GPIO6
#define VSEL2 2 // For GPIO6
#define SYNC_OUT2 3 // For GPIO6
#define NTC2 5 // For GPIO6
#define I2C_SLV_SDA 6 // For GPIO6

#define I2C_SLV_SCL 0 // For GPIO5
#define SYNC_OUT1 3 // For GPIO5
#define VBUS_DETECT 4 // For GPIO5


// Hardware I2C line on different microcontrollers

class MPS {
    public:
        // Constructor
        MPS(uint8_t I2C_SCL, uint8_t  I2C_SDA);  // Defines different microcontroller
          // Methods
          void begin(); 
          void end();
    protected:
        // Variables 
        uint8_t _SCL;
        uint8_t _SDA;
          // Methods
          uint16_t readRegister(uint8_t address, uint8_t registerAddress); // Function returns 16bit value
          void writeRegister(uint8_t address, uint8_t registerAddress, uint16_t value);
};

class MP5031 : public MPS {
  public:
    // Constructor
      MP5031(uint8_t adressOfDevice = STD_ADR);  // Start the transmission at chosen adress
        // Methods
          uint8_t getSoftwareRevision();
          void setPowerDataObject(int profile, int mode);
          void setPowerDataObjectMode(int profile, int mode);
          void setPowerDataObjectCurrent(int profile, int current); // in miliampers
          void setPowerDataObjectVoltage(int profile, int minVoltage, int maxVoltage = 0); // in milivolts
          uint8_t getSuffixCode();
          void setLowVoltageDetectionResponse(int response);
          void setLegacyChargingMode(int mode);
          void setNTCPowerSharingFunction(int mode);
          void setChargingDataPortMode(int mode);
          void setSlaveAddress(int address);
          void setTypeCMode(int current);
          void setSlaveDevice(int device);
          void reset();
          void update();
          void setUSBSuspend(int mode);
          void setUSBCommunication(int mode);
          void setLPSCompliance(int mode);
          void setTouchCurrent(int mode);
          void setTouchTemperature(int mode);
          void setChunkedBit(int mode);
          void setAmountOfSentSourceCapability(int time);
          void setVDRV(int mode);
          void setOperationMode(int mode);
          void setFrequencyDithering(int mode);
          void setTimerDelay(int mode);
          void setGPIO4Functionality(int mode);
          void setGPIO3Functionality(int mode);
          void setGPIO2Functionality(int mode);
          void setGPIO1Functionality(int mode);
          void setNTCBehaviour(int mode);
          void setUVLOThreshold(int mode);
          void setVDRVFunctionality(int mode);
          void setGPIO7Functionality(int mode);
          void setGPIO6Functionality(int mode);
          void setGPIO5Functionality(int mode);
          void setSinkPowerDataObjectPowerRating(int value);
          void setSourcePowerDataObjectPowerRating(int value);
          uint16_t getSinkRequestedVoltage(); // in milivolts
          uint8_t checkDataLineStatus();
          uint8_t checkAttachStatus();
          uint8_t checkOverTemperatureEvent();
          uint8_t checkContract();
          uint8_t checkLoadShedding();
          uint8_t checkCableCapability();
          uint8_t checkMismatchEvent();
          uint8_t checkSinkPowerDataObjectDemand();
          uint8_t getSinkRequestedCurrent(); // in miliamperes
          uint8_t getVendorIdentificationNumber(); // in miliamperes
          void setDigitalClock(int mode); //

  private:
    // Variables
    uint8_t _address;
    uint8_t _resolution;
};

#endif

          

