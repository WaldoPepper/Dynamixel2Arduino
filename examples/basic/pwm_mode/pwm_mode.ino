/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <Dynamixel2Arduino.h>

// Please modify it to suit your hardware.
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560) // When using DynamixelShield
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
  #define DXL_SERIAL   Serial
  #define DEBUG_SERIAL soft_serial
  const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
#elif ARDUINO_OpenCM904 // Official ROBOTIS board with DXL circuit.
  #define DXL_SERIAL   Serial3 //OpenCM9.04 EXP Board's DXL port Serial. (To use the DXL port on the OpenCM 9.04 board, you must use Serial1 for Serial. And because of the OpenCM 9.04 driver code, you must call Serial1.setDxlMode(true); before dxl.begin();.)
  #define DEBUG_SERIAL Serial
  const uint8_t DXL_DIR_PIN = 22; //OpenCM9.04 EXP Board's DIR PIN. (To use the DXL port on the OpenCM 9.04 board, you must use 28 for DIR PIN.)  
#elif ARDUINO_OpenCR // Official ROBOTIS board with DXL circuit.
  // For OpenCR, there is a DXL Power Enable pin, so you must initialize and control it.
  // Reference link : https://github.com/ROBOTIS-GIT/OpenCR/blob/master/arduino/opencr_arduino/opencr/libraries/DynamixelSDK/src/dynamixel_sdk/port_handler_arduino.cpp#L78
  #define DXL_SERIAL   Serial3
  #define DEBUG_SERIAL Serial
  const uint8_t DXL_DIR_PIN = 84; //OpenCM9.04 EXP Board's DIR PIN. (To use the DXL port on the OpenCM 9.04 board, you must use 28 for DIR PIN.)    
#else // When using DynamixelShield
  #define DXL_SERIAL   Serial1
  #define DEBUG_SERIAL Serial
  const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
#endif

const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

void setup() {
  // put your setup code here, to run once:
  
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_PWM);
  dxl.torqueOn(DXL_ID);
}

void loop() {
  // put your main code here, to run repeatedly:
   
  // Please refer to e-Manual(http://emanual.robotis.com) for available range of value. 
  // Set Goal PWM using RAW unit
  dxl.setGoalPWM(DXL_ID, 300);
  delay(1000);
  // Print present PWM
  DEBUG_SERIAL.print("Present PWM(raw) : ");
  DEBUG_SERIAL.println(dxl.getPresentPWM(DXL_ID));
  delay(1000);

  // Set Goal PWM using percentage (-100.0 [%] ~ 100.0 [%])
  dxl.setGoalPWM(DXL_ID, -40.8, UNIT_PERCENT);
  delay(1000);
  DEBUG_SERIAL.print("Present PWM(ratio) : ");
  DEBUG_SERIAL.println(dxl.getPresentPWM(DXL_ID, UNIT_PERCENT));
  delay(1000);
}
