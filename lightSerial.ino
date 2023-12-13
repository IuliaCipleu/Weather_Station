

// // // #include <Wire.h>
// // // #include <SparkFun_APDS9960.h>

// // // // Pins
// // // #define APDS9960_INT    2  // Needs to be an interrupt pin
// // // #define LED_PIN         13 // LED for showing interrupt

// // // // Constants
// // // #define LIGHT_INT_HIGH  1000 // High light level for interrupt
// // // #define LIGHT_INT_LOW   10   // Low light level for interrupt

// // // // Global variables
// // // SparkFun_APDS9960 apds = SparkFun_APDS9960();
// // // uint16_t ambient_light = 0;
// // // uint16_t red_light = 0;
// // // uint16_t green_light = 0;
// // // uint16_t blue_light = 0;
// // // int isr_flag = 0;
// // // uint16_t threshold = 0;

// // // void setup() {
  
// // //   // Set LED as output
// // //   pinMode(LED_PIN, OUTPUT);
// // //   pinMode(APDS9960_INT, INPUT);
  
// // //   // Initialize Serial port
// // //   Serial.begin(9600);
// // //   Serial.println();
// // //   Serial.println(F("-------------------------------------"));
// // //   Serial.println(F("SparkFun APDS-9960 - Light Interrupts"));
// // //   Serial.println(F("-------------------------------------"));
  
// // //   // Initialize interrupt service routine
// // //   attachInterrupt(0, interruptRoutine, FALLING);
  
// // //   // Initialize APDS-9960 (configure I2C and initial values)
// // //   if ( apds.init() ) {
// // //     Serial.println(F("APDS-9960 initialization complete"));
// // //   } else {
// // //     Serial.println(F("Something went wrong during APDS-9960 init!"));
// // //   }
  
// // //   // Set high and low interrupt thresholds
// // //   if ( !apds.setLightIntLowThreshold(LIGHT_INT_LOW) ) {
// // //     Serial.println(F("Error writing low threshold"));
// // //   }
// // //   if ( !apds.setLightIntHighThreshold(LIGHT_INT_HIGH) ) {
// // //     Serial.println(F("Error writing high threshold"));
// // //   }
  
// // //   // Start running the APDS-9960 light sensor (no interrupts)
// // //   if ( apds.enableLightSensor(false) ) {
// // //     Serial.println(F("Light sensor is now running"));
// // //   } else {
// // //     Serial.println(F("Something went wrong during light sensor init!"));
// // //   }
  
// // //   // Read high and low interrupt thresholds
// // //   if ( !apds.getLightIntLowThreshold(threshold) ) {
// // //     Serial.println(F("Error reading low threshold"));
// // //   } else {
// // //     Serial.print(F("Low Threshold: "));
// // //     Serial.println(threshold);
// // //   }
// // //   if ( !apds.getLightIntHighThreshold(threshold) ) {
// // //     Serial.println(F("Error reading high threshold"));
// // //   } else {
// // //     Serial.print(F("High Threshold: "));
// // //     Serial.println(threshold);
// // //   }
  
// // //   // Enable interrupts
// // //   if ( !apds.setAmbientLightIntEnable(1) ) {
// // //     Serial.println(F("Error enabling interrupts"));
// // //   }
  
// // //   // Wait for initialization and calibration to finish
// // //   delay(500);
// // // }

// // // void loop() {
  
// // //   // If interrupt occurs, print out the light levels
// // //   if ( isr_flag == 1 ) {
    
// // //     // Read the light levels (ambient, red, green, blue) and print
// // //     if (  !apds.readAmbientLight(ambient_light) ||
// // //           !apds.readRedLight(red_light) ||
// // //           !apds.readGreenLight(green_light) ||
// // //           !apds.readBlueLight(blue_light) ) {
// // //       Serial.println("Error reading light values");
// // //     } else {
// // //       Serial.print("Interrupt! Ambient: ");
// // //       Serial.print(ambient_light);
// // //       Serial.print(" R: ");
// // //       Serial.print(red_light);
// // //       Serial.print(" G: ");
// // //       Serial.print(green_light);
// // //       Serial.print(" B: ");
// // //       Serial.println(blue_light);
// // //     }
    
// // //     // Turn on LED for a half a second
// // //     digitalWrite(LED_PIN, HIGH);
// // //     delay(500);
// // //     digitalWrite(LED_PIN, LOW);
    
// // //     // Reset flag and clear APDS-9960 interrupt (IMPORTANT!)
// // //     isr_flag = 0;
// // //     if ( !apds.clearAmbientLightInt() ) {
// // //       Serial.println("Error clearing interrupt");
// // //     }
    
// // //   }
// // // }

// // // void interruptRoutine() {
// // //   isr_flag = 1;
// // // }
// // #include <Wire.h>
// // #include <APDS9960.h> //Download Code Library Below
// // #define APDS9960_INT    2 // Needs to be an interrupt pin
// // APDS9960 apds = APDS9960();
// // int isr_flag = 0;

// // void setup() {
// //   pinMode(APDS9960_INT, INPUT);   // Set interrupt pin as input
// //   Serial.begin(9600); //Initialize Serial Port
// //   Serial.println();
// //   Serial.println("--------------------------------");
// //   Serial.println("14CORE | GESTURE TEST W/D APDS-9960");
// //   Serial.println("--------------------------------");
  
// //   attachInterrupt(0, interruptRoutine, FALLING);   // Initialize interrupt service routine
// //   if ( apds.init() ) { 
// //     Serial.println("APDS-9960 initialization complete"); // Initialize APDS-9960 (configure I2C and initial values)
// //   } else {
// //     Serial.println("Error APDS Initialization");
// //   }
// //   if ( apds.enableGestureSensor(true) ) {
// //     Serial.println("Gesture sensor is now running");
// //   } else {
// //     Serial.println("Error APDS Initialization");
// //   }
// // }

// // void loop() {
// //   if( isr_flag == 1 ) {
// //     detachInterrupt(0);
// //     handleGesture();
// //     isr_flag = 0;
// //     attachInterrupt(0, interruptRoutine, FALLING);
// //   }
// // }

// // void interruptRoutine() {
// //   isr_flag = 1;
// // }

// // void handleGesture() {
// //     if ( apds.isGestureAvailable() ) {
// //     switch ( apds.readGesture() ) {
// //       case DIR_UP:
// //         Serial.println("UP");
// //         break;
// //       case DIR_DOWN:
// //         Serial.println("DOWN");
// //         break;
// //       case DIR_LEFT:
// //         Serial.println("LEFT");
// //         break;
// //       case DIR_RIGHT:
// //         Serial.println("RIGHT");
// //         break;
// //       case DIR_NEAR:
// //         Serial.println("NEAR");
// //         break;
// //       case DIR_FAR:
// //         Serial.println("FAR");
// //         break;
// //       default:
// //         Serial.println("NONE");
// //     }
// //   }
// // }
// /****************************************************************
// AmbientLightSensor.ino
// APDS-9930 Ambient light and proximity sensor
// Davide Depau
// December 11, 2015
// https://github.com/Davideddu/APDS9930

// Shawn Hymel @ SparkFun Electronics
// October 15, 2014
// https://github.com/sparkfun/APDS-9930_RGB_and_Gesture_Sensor

// Tests thembient light sensing abilities of the 
// APDS-9930. Configures APDS-9930 over I2C and polls the sensor for
// ambient light levels, which are displayed over the 
// serial console.

// Hardware Connections:

// IMPORTANT: The APDS-9930 can only accept 3.3V!
 
//  Arduino Pin  APDS-9930 Board  Function
 
//  3.3V         VCC              Power
//  GND          GND              Ground
//  A4           SDA              I2C Data
//  A5           SCL              I2C Clock

// Resources:
// Include Wire.h and APDS-9930.h

// Development environment specifics:
// Written in Arduino 1.6.5
// Tested with Arduino Uno and Mega.

// This code is beerware; if you see me (or any other SparkFun 
// employee) at the local, and you've found our code helpful, please
// buy us a round!

// Distributed as-is; no warranty is given.
// ****************************************************************/

// #define DUMP_REGS

// #include <Wire.h>
// #include <APDS9930.h>

// // Global Variables
// APDS9930 apds = APDS9930();
// float ambient_light = 0; // can also be an unsigned long
// uint16_t ch0 = 0;
// uint16_t ch1 = 1;

// void setup() {
//   //analogReference(EXTERNAL);
  
//   // Initialize Serial port
//   Serial.begin(9600);
//   Serial.println();
//   Serial.println(F("--------------------------------"));
//   Serial.println(F("APDS-9930 - Ambient light sensor"));
//   Serial.println(F("--------------------------------"));
  
//   // Initialize APDS-9930 (configure I2C and initial values)
//   if ( apds.init() ) {
//     Serial.println(F("APDS-9930 initialization complete"));
//   } else {
//     Serial.println(F("Something went wrong during APDS-9930 init!"));
//   }
  
//   // Start running the APDS-9930 light sensor (no interrupts)
//   if ( apds.enableLightSensor(false) ) {
//     Serial.println(F("Light sensor is now running"));
//   } else {
//     Serial.println(F("Something went wrong during light sensor init!"));
//   }

// #ifdef DUMP_REGS
//   /* Register dump */
//   uint8_t reg;
//   uint8_t val;

//   for(reg = 0x00; reg <= 0x19; reg++) {
//     if( (reg != 0x10) && \
//         (reg != 0x11) )
//     {
//       apds.wireReadDataByte(reg, val);
//       Serial.print(reg, HEX);
//       Serial.print(": 0x");
//       Serial.println(val, HEX);
//     }
//   }
//   apds.wireReadDataByte(0x1E, val);
//   Serial.print(0x1E, HEX);
//   Serial.print(": 0x");
//   Serial.println(val, HEX);
// #endif

//   // Wait for initialization and calibration to finish
//   delay(500);
// }

// void loop() {
  
//   // Read the light levels (ambient, red, green, blue)
//   if (  !apds.readAmbientLightLux(ambient_light) ||
//         !apds.readCh0Light(ch0) || 
//         !apds.readCh1Light(ch1) ) {
//     Serial.println(F("Error reading light values"));
//   } else {
//     Serial.print(F("Ambient: "));
//     Serial.print(ambient_light);
//     Serial.print(F("  Ch0: "));
//     Serial.print(ch0);
//     Serial.print(F("  Ch1: "));
//     Serial.println(ch1);
//   }
  
//   // Wait 1 second before next reading
//   delay(1000);
// }
  /*
  APDS-9930-Proximity-And-Ambient-Light
  modified on 02 Nov 2020
  by Amir Mohammad Shojaee @ Electropeak
  Home

  Based on Github.com Library Example
*/

#define DUMP_REGS
 
#include <Wire.h>
#include <APDS9930.h>
 
// Global Variables
APDS9930 apds = APDS9930();
float ambient_light = 0; // can also be an unsigned long

 
void setup() 
{
 
  // Initialize Serial port
  Serial.begin(9600);
  // while (!Serial);             // Leonardo: wait for serial monitor
  // Serial.println("\nI2C Scanner");
  Serial.println();
 delay(500);
  // Initialize APDS-9930 (configure I2C and initial values)
  if ( apds.init() ) 
  {
    Serial.println(F("APDS-9930 initialization complete"));
  } 
  else 
  {
    Serial.println(F("Something went wrong during APDS-9930 init!"));
  }
 
  // Start running the APDS-9930 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) 
  {
    Serial.println(F("Light sensor is now running"));
  } 
  else 
  {
    Serial.println(F("Something went wrong during light sensor init!"));
  }
 
  // Wait for initialization and calibration to finish
  delay(500);
}
 
//  void loop()
// {
//   byte error, address;
//   int nDevices;

//   Serial.println("Scanning...");

//   nDevices = 0;
//   for(address = 1; address < 127; address++ ) 
//   {
//     // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();

//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address<16) 
//         Serial.print("0");
//       Serial.print(address,HEX);
//       Serial.println("  !");

//       nDevices++;
//     }
//     else if (error==4) 
//     {
//       Serial.print("Unknown error at address 0x");
//       if (address<16) 
//         Serial.print("0");
//       Serial.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");

//   delay(5000);           // wait 5 seconds for next scan
// }
 
void loop() 
{
 
  // Read the light levels (ambient, red, green, blue)
  if (  !apds.readAmbientLightLux(ambient_light)) {
    Serial.println(F("Error reading light values"));
  } 
  else 
  {
    Serial.print(F("Ambient: "));
    Serial.println(ambient_light);
    
  }
 
  // Wait 1 second before next reading
  delay(1000);
}
