#include "SimpleMP5031.h"
#include <Arduino.h>
#include <Wire.h>

  MPS::MPS(uint8_t I2C_SCL, uint8_t I2C_SDA) {
    _SCL = I2C_SCL;
    _SDA = I2C_SDA;
  }

  void MPS::begin() {
    Wire.begin(_SDA, _SCL);
  }

  void MPS::end() {
    Wire.end();
  }

  uint16_t MPS::readRegister(uint8_t address, uint8_t registerAddress) {
    Wire.beginTransmission(address);  // Start comunication with slave 
    Wire.write(registerAddress);  // Get to the register
    Wire.endTransmission(false);  // Refresh the connection
    Wire.requestFrom(address, (uint8_t)2); // Ask for 2 byte of data
    uint16_t value = 0;
      if (Wire.available() >= 2) {
        value = Wire.read(); 
        value |= (Wire.read() << 8);
      }
    return value;
  }

  void MPS::writeRegister(uint8_t address, uint8_t registerAddress, uint16_t value) {
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(value & 0xFF); 
    Wire.write(value >> 8);     
    Wire.endTransmission();
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  MP5031::MP5031(uint8_t adressOfDevice) : MPS(_SCL, _SDA) {
    _address = adressOfDevice;
  }

  uint8_t MP5031::getSoftwareRevision() {
    uint16_t registerValue = readRegister(_address, PDO_TYPE);
    registerValue = registerValue >> 10;
    return registerValue;
  }

  void MP5031::setPowerDataObject(int profile, int mode) {
    uint16_t registerValue = readRegister(_address, PDO_TYPE);
      switch (profile) {
        case 5:
          if (mode == 0) {
            registerValue = (registerValue & 0xFF7F) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFF7F) | (0x80); 
          }
          break;
        case 4:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFFB) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFBF) | (0x40); 
          }
          break;
        case 3:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFDF) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFDF) | (0x20); 
          }
          break;
        case 2:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFEF) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFEF) | (0x10); 
          }
          break;
        case 1:
          if (mode == 0) {
            registerValue = (registerValue & 0xFF0F) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFF0F) | (0xF0); 
          }
          break;
      }
      writeRegister(_address, PDO_TYPE, registerValue);
  }

  void MP5031::setPowerDataObjectMode(int profile, int mode) {
    uint16_t registerValue = readRegister(_address, PDO_TYPE);
      switch (profile) {
        case 5:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFF7) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFF7) | (0x08); 
          }
          break;
        case 4:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFFB) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFFB) | (0x04); 
          }
          break;
        case 3:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFFD) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFFD) | (0x02); 
          }
          break;
        case 2:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFFE) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFFE) | (0x01); 
          }
          break;
        case 1:
          if (mode == 0) {
            registerValue = (registerValue & 0xFFF0) | (0x00); 
          }
          else if (mode == 1) {
            registerValue = (registerValue & 0xFFF0) | (0x0F); 
          }
          break;
      }
      writeRegister(_address, PDO_TYPE, registerValue);
  }
 
  void MP5031::setPowerDataObjectCurrent(int profile, int current) {
    uint16_t registerValue = readRegister(_address, PDO_TYPE);
      switch (profile) {
        case 5:
          registerValue = (registerValue & 0x0008) >> 3;
            if (registerValue == 0) {
              registerValue = ((current / 10) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((current / 50) & 0x007F);
            }
          writeRegister(_address, PDO_I5, registerValue);  
          break;
        case 4:
          registerValue = (registerValue & 0x0004) >> 2;
            if (registerValue == 0) {
              registerValue = ((current / 10) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((current / 50) & 0x007F);
            }
          writeRegister(_address, PDO_I4, registerValue);
          break;
        case 3:
          registerValue = (registerValue & 0x0002) >> 1;
            if (registerValue == 0) {
              registerValue = ((current / 10) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((current / 50) & 0x007F);
            }
          writeRegister(_address, PDO_I3, registerValue);
          break;
        case 2:
          registerValue = (registerValue & 0x0001);
            if (registerValue == 0) {
              registerValue = ((current / 10) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((current / 50) & 0x007F);
            }
          writeRegister(_address, PDO_I2, registerValue);
          break;
        case 1:
          registerValue = readRegister(_address, PDO_I1);
          registerValue = (registerValue & 0xFC00) | (current / 10);
          writeRegister(_address, PDO_I1, registerValue);
          break;
      }
  }

  void MP5031::setPowerDataObjectVoltage(int profile, int minVoltage, int maxVoltage) {
    uint16_t registerValue = readRegister(_address, PDO_TYPE);
      switch (profile) {
        case 5:
          registerValue = (registerValue & 0x0008) >> 3;
            if (registerValue == 0) {
              registerValue = ((minVoltage / 50) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((maxVoltage / 100) << 8) | (minVoltage / 100);
            }
          writeRegister(_address, PDO_V5, registerValue);
          break;
        case 4:
          registerValue = (registerValue & 0x0004) >> 2;
            if (registerValue == 0) {
              registerValue = ((minVoltage / 50) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((maxVoltage / 100) << 8) | (minVoltage / 100);
            }
          writeRegister(_address, PDO_V4, registerValue);
          break;
        case 3:
          registerValue = (registerValue & 0x0002) >> 1;
            if (registerValue == 0) {
              registerValue = ((minVoltage / 50) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((maxVoltage / 100) << 8) | (minVoltage / 100);
            }
          writeRegister(_address, PDO_V3, registerValue);
          break;
        case 2:
          registerValue = (registerValue & 0x0001);
            if (registerValue == 0) {
              registerValue = ((minVoltage / 50) & 0x03FF);
            }
            else if (registerValue == 1) {
              registerValue = ((maxVoltage / 100) << 8) | (minVoltage / 100);
            }
          writeRegister(_address, PDO_V2, registerValue);
          break;
      }
  }

  uint8_t MP5031::getSuffixCode() {
    uint16_t registerValue = readRegister(_address, PDO_I1);
    registerValue = registerValue >> 10;
    return registerValue;
  }

  void MP5031::setLowVoltageDetectionResponse(int response) {
    uint16_t registerValue = readRegister(_address, CTL1);
      switch (response) {
        case 0:
          registerValue = (registerValue & 0x7EFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0x7EFF) | (0x0100);
          break;
        case 2:
          registerValue = (registerValue & 0x7EFF) | (0x8000);
          break;
      }
    writeRegister(_address, CTL1, registerValue);
  }

  void MP5031::setLegacyChargingMode(int mode) {
    uint16_t registerValue = readRegister(_address, CTL1);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xB7FF) | (0x0800);
          break;
        case 1:
          registerValue = (registerValue & 0xB7FF) | (0x0000);
          break;
        case 2:
          registerValue = (registerValue & 0xB7FF) | (0x4000);
          break;
      }
    writeRegister(_address, CTL1, registerValue);
  }

  void MP5031::setNTCPowerSharingFunction(int mode) {
    uint16_t registerValue = readRegister(_address, CTL1);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xDFFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xDFFF) | (0x0200);
          break;
      }
    writeRegister(_address, CTL1, registerValue);
  }

  void MP5031::setChargingDataPortMode(int mode) {
    uint16_t registerValue = readRegister(_address, CTL1);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xEFFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xEFFF) | (0x0100);
          break;
      }
    writeRegister(_address, CTL1, registerValue);
  }

  void MP5031::setSlaveAddress(int address) {
    uint16_t registerValue = readRegister(_address, CTL1);
    registerValue = (registerValue & 0xFF0F) | ((address - 20) << 4);
    writeRegister(_address, CTL1, registerValue);
  }

  void MP5031::setTypeCMode(int current) {
    uint16_t registerValue = readRegister(_address, CTL1);
      switch (current) {
        case 3:
          registerValue = (registerValue & 0xFFF7) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xFFF7) | (0x0008);
          break;
      }
    writeRegister(_address, CTL1, registerValue);
  }

  void MP5031::setSlaveDevice(int device) {
    uint16_t registerValue = readRegister(_address, CTL1);
      switch (device) {
        case 0:
          registerValue = (registerValue & 0xFFF8) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xFFF8) | (0x0001);
          break;
        case 2:
          registerValue = (registerValue & 0xFFF8) | (0x0002);
          break;
        case 3:
          registerValue = (registerValue & 0xFFF8) | (0x0003);
          break;
        case 4:
          registerValue = (registerValue & 0xFFF8) | (0x0004);
          break;
        case 5:
          registerValue = (registerValue & 0xFFF8) | (0x0005);
          break;
      }
    writeRegister(_address, CTL1, registerValue);
  }

  void MP5031::reset() {
    uint16_t registerValue = readRegister(_address, CTL2);
    registerValue = (registerValue & 0x7FFF) | (0x8000);
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::update() {
    uint16_t registerValue = readRegister(_address, CTL2);
    registerValue = (registerValue & 0xBFFF) | (0x4000);
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setUSBSuspend(int mode) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xDFFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xDFFF) | (0x2000);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setUSBCommunication(int mode) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xEFFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xEFFF) | (0x1000);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setLPSCompliance(int mode) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xF1FF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xF7FF) | (0x0800);
          break;
        case 2:
          registerValue = (registerValue & 0xFBFF) | (0x0400);
          break;
        case 3:
          registerValue = (registerValue & 0xFDFF) | (0x0200);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setTouchCurrent(int mode) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFE3F) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xFFBF) | (0x0040);
          break;
        case 2:
          registerValue = (registerValue & 0xFF7F) | (0x0080);
          break;
        case 3:
          registerValue = (registerValue & 0xFEFF) | (0x0100);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setTouchTemperature(int mode) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (mode) {
        case 1:
          registerValue = (registerValue & 0xFFCF) | (0x0000);
          break;
        case 2:
          registerValue = (registerValue & 0xFFEF) | (0x0010);
          break;
        case 3:
          registerValue = (registerValue & 0xFFDF) | (0x0020);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setChunkedBit(int mode) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFFF7) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xFFF7) | (0x0008);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setAmountOfSentSourceCapability(int time) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (time) {
        case 0:
          registerValue = (registerValue & 0xFFFD) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xFFFD) | (0x0002);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setVDRV(int mode) {
    uint16_t registerValue = readRegister(_address, CTL2);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFFFE) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xFFFE) | (0x0001);
          break;
      }
    writeRegister(_address, CTL2, registerValue);
  }

  void MP5031::setOperationMode(int mode) {
    uint16_t registerValue = readRegister(_address, CTL3);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0x7FFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0x7FFF) | (0x8000);
          break;
      }
    writeRegister(_address, CTL3, registerValue);
  }

  void MP5031::setFrequencyDithering(int mode) {
    uint16_t registerValue = readRegister(_address, CTL3);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xBFFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xBFFF) | (0x4000);
          break;
      }
    writeRegister(_address, CTL3, registerValue);
  }

  void MP5031::setTimerDelay(int mode) {
    uint16_t registerValue = readRegister(_address, CTL3);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xCFFF) | (0x0000);
          break;
        case 10:
          registerValue = (registerValue & 0xCFFF) | (0x0800);
          break;
        case 22:
          registerValue = (registerValue & 0xCFFF) | (0x1000);
          break;
        case 40:
          registerValue = (registerValue & 0xCFFF) | (0x1800);
          break;
        case 80:
          registerValue = (registerValue & 0xCFFF) | (0x2000);
          break;
        case 120:
          registerValue = (registerValue & 0xCFFF) | (0x2800);
          break;
      }
    writeRegister(_address, CTL3, registerValue);
  }

  void MP5031::setGPIO4Functionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL3);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xF9FF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xF9FF) | (0x0200);
          break;
        case 5:
          registerValue = (registerValue & 0xF9FF) | (0x0400);
          break;
        case 3:
          registerValue = (registerValue & 0xF9FF) | (0x0600);
          break;
      }
    writeRegister(_address, CTL3, registerValue);
  }

  void MP5031::setGPIO3Functionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL3);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFE3F) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xFE3F) | (0x0040);
          break;
        case 2:
          registerValue = (registerValue & 0xFE3F) | (0x0080);
          break;
        case 3:
          registerValue = (registerValue & 0xFE3F) | (0x00C0);
          break;
        case 4:
          registerValue = (registerValue & 0xFE3F) | (0x0100);
          break;
        case 5:
          registerValue = (registerValue & 0xFE3F) | (0x0140);
          break;
        case 6:
          registerValue = (registerValue & 0xFE3F) | (0x0180);
          break;
        case 7:
          registerValue = (registerValue & 0xFE3F) | (0x01C0);
          break;
      }
    writeRegister(_address, CTL3, registerValue);
  }

  void MP5031::setGPIO2Functionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL3);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFFC7) | (0x0000);
          break;
        case 4:
          registerValue = (registerValue & 0xFFC7) | (0x0008);
          break;
        case 1:
          registerValue = (registerValue & 0xFFC7) | (0x0010);
          break;
        case 2:
          registerValue = (registerValue & 0xFFC7) | (0x0030);
          break;
        case 3:
          registerValue = (registerValue & 0xFFC7) | (0x0038);
          break;
      }
    writeRegister(_address, CTL3, registerValue);
  }

  void MP5031::setGPIO1Functionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL3);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFFF8) | (0x0000);
          break;
        case 2:
          registerValue = (registerValue & 0xFFF8) | (0x0001);
          break;
        case 1:
          registerValue = (registerValue & 0xFFF8) | (0x0002);
          break;
        case 3:
          registerValue = (registerValue & 0xFFF8) | (0x0003);
          break;
        case 4:
          registerValue = (registerValue & 0xFFF8) | (0x0004);
          break;
      }
    writeRegister(_address, CTL3, registerValue);
  }

  void MP5031::setNTCBehaviour(int mode) {
    uint16_t registerValue = readRegister(_address, CTL4);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0x7FFF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0x7FFF) | (0x8000);
          break;
      }
    writeRegister(_address, CTL4, registerValue);
  }

    void MP5031::setUVLOThreshold(int mode) {
    uint16_t registerValue = readRegister(_address, CTL4);
      switch (mode) {
        case 3:
          registerValue = (registerValue & 0xBFFF) | (0x0000);
          break;
        case 5:
          registerValue = (registerValue & 0xBFFF) | (0x4000);
          break;
      }
    writeRegister(_address, CTL4, registerValue);
  }

  void MP5031::setVDRVFunctionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL4);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xF9FF) | (0x0000);
          break;
        case 1:
          registerValue = (registerValue & 0xF9FF) | (0x0200);
          break;
      }
    writeRegister(_address, CTL4, registerValue);
  }

  void MP5031::setGPIO7Functionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL4);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFE3F) | (0x0000);
          break;
        case 2:
          registerValue = (registerValue & 0xFE3F) | (0x0040);
          break;
        case 1:
          registerValue = (registerValue & 0xFE3F) | (0x00C0);
          break;
        case 3:
          registerValue = (registerValue & 0xFE3F) | (0x0100);
          break;
        case 4:
          registerValue = (registerValue & 0xFE3F) | (0x0140);
          break;
      }
    writeRegister(_address, CTL4, registerValue);
  }

  void MP5031::setGPIO6Functionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL4);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFFC7) | (0x0000);
          break;
        case 2:
          registerValue = (registerValue & 0xFFC7) | (0x0008);
          break;
        case 4:
          registerValue = (registerValue & 0xFFC7) | (0x0010);
          break;
        case 1:
          registerValue = (registerValue & 0xFFC7) | (0x0018);
          break;
        case 3:
          registerValue = (registerValue & 0xFFC7) | (0x0020);
          break;
        case 5:
          registerValue = (registerValue & 0xFFC7) | (0x0028);
          break;
        case 6:
          registerValue = (registerValue & 0xFFC7) | (0x0030);
          break;
      }
    writeRegister(_address, CTL4, registerValue);
  }

  void MP5031::setGPIO5Functionality(int mode) {
    uint16_t registerValue = readRegister(_address, CTL4);
      switch (mode) {
        case 0:
          registerValue = (registerValue & 0xFFF8) | (0x0000);
          break;
        case 7:
          registerValue = (registerValue & 0xFFF8) | (0x0001);
          break;
        case 3:
          registerValue = (registerValue & 0xFFF8) | (0x0002);
          break;
        case 1:
          registerValue = (registerValue & 0xFFF8) | (0x0003);
          break;
        case 2:
          registerValue = (registerValue & 0xFFF8) | (0x0005);
          break;
        case 4:
          registerValue = (registerValue & 0xFFF8) | (0x0007);
          break;
      }
    writeRegister(_address, CTL4, registerValue);
  }

  void MP5031::setSinkPowerDataObjectPowerRating(int value) {
    uint16_t registerValue = readRegister(_address, PWRSHA);
    registerValue = (registerValue & 0x00FF) | (value << 8);
    writeRegister(_address, PWRSHA, registerValue);
  }

  void MP5031::setSourcePowerDataObjectPowerRating(int value) {
    uint16_t registerValue = readRegister(_address, PWRSHA);
    registerValue = (registerValue & 0xFF00) | (value);
    writeRegister(_address, PWRSHA, registerValue);
  }

  uint16_t MP5031::getSinkRequestedVoltage() {
    uint16_t registerValue = readRegister(_address, STATUS1);
    return (registerValue >> 5) * 20;
  }

  uint8_t MP5031::checkDataLineStatus() {
    uint16_t registerValue = readRegister(_address, STATUS1);
    registerValue = ((registerValue & 0x0010) >> 4);
    return registerValue;
  }

  uint8_t MP5031::checkAttachStatus() {
    uint16_t registerValue = readRegister(_address, STATUS1);
    registerValue = ((registerValue & 0x0008) >> 3);
    return registerValue;
  }

  uint8_t MP5031::checkOverTemperatureEvent() {
    uint16_t registerValue = readRegister(_address, STATUS1);
    registerValue = ((registerValue & 0x0004) >> 2);
    return registerValue;
  }

  uint8_t MP5031::checkContract() {
    uint16_t registerValue = readRegister(_address, STATUS1);
    registerValue = ((registerValue & 0x0002) >> 1);
    return registerValue;
  }

  uint8_t MP5031::checkLoadShedding() {
    uint16_t registerValue = readRegister(_address, STATUS1);
    registerValue = (registerValue & 0x0001);
    return registerValue;
  }

  uint8_t MP5031::checkCableCapability() {
    uint16_t registerValue = readRegister(_address, STATUS2);
    registerValue = ((registerValue & 0x4000) >> 14);
      if (registerValue == 0) {
        registerValue = 3;
      }
      else {
        registerValue = 5;
      }
    return registerValue;
  }

  uint8_t MP5031::checkMismatchEvent() {
    uint16_t registerValue = readRegister(_address, STATUS2);
    registerValue = ((registerValue & 0x2000) >> 13);
    return registerValue;
  }

  uint8_t MP5031::checkSinkPowerDataObjectDemand () {
    uint16_t registerValue = readRegister(_address, STATUS2);
    registerValue = ((registerValue & 0x1C00) >> 10);
    return registerValue;
  }

  uint8_t MP5031::getSinkRequestedCurrent() {
    uint16_t registerValue = readRegister(_address, STATUS2);
    registerValue = ((registerValue & 0x03FC) >> 2) * 50;
    return registerValue;
  }

  uint8_t MP5031::getVendorIdentificationNumber() {
    uint16_t registerValue = readRegister(_address, ID);
    return registerValue >> 12;
  }

  void MP5031::setDigitalClock(int mode) {
    if (mode == 0) {
      writeRegister(_address, CLK_ON, 0x0000);
    }
    else if (mode == 1) {
      writeRegister(_address, CLK_ON, 0x55AA);
    }
  }