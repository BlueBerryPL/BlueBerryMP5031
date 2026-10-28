#include "SimpleMP5031.h"
#include "Wire.h"

#define ENABLE_MP4247 1 
#define ENABLE_MP5031 2 
#define ENABLE_I2C 3 

MP5031 USB(STD_ADR);

void setup() {

  pinMode(ENABLE_MP4247, OUTPUT);
  pinMode(ENABLE_MP5031, OUTPUT);
  pinMode(ENABLE_I2C, OUTPUT);

  digitalWrite(ENABLE_MP4247, LOW);
  digitalWrite(ENABLE_MP5031, LOW);
  digitalWrite(ENABLE_I2C, HIGH);

  Wire.begin();

  USB.reset();
  USB.setLegacyChargingMode(ALL);
  USB.setSlaveDevice(MP424X); // This example uses MP4247 buck - boost converter, designed to cooperate with MP5031
  USB.setGPIO7Functionality(ATTACH);
  USB.setGPIO4Functionality(NTC2);
  USB.setGPIO3Functionality(EN);
  USB.setGPIO2Functionality(POL);
  USB.setGPIO1Functionality(INT);
  USB.setDigitalClock(ENABLE);
  USB.update();

  Wire.end();

  delay(1); // Wait for MP5031 to make changes in registers

  digitalWrite(ENABLE_MP4247, HIGH);
  digitalWrite(ENABLE_MP5031, HIGH);
  digitalWrite(ENABLE_I2C, LOW);

}

void loop() {

// MP5031 works in an autonomous mode, you can put your code here.

}
