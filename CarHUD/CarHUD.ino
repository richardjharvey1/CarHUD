/**
 * CarHUD
 * Shawn Hymel @ SparkFun Electronics
 * October 6, 2015
 *
 * Uses CAN-BUS Shield to read MPH and RPM. Displays MPH on
 * 7-segment display and RPM on 8 LEDs.
 *
 * License: This code is beerware; if you see me (or any other 
 * SparkFun employee) at the local, and you've found our code
 * helpful, please buy us a round!
 *
 * Distributed as-is; no warranty is given.
 */
 
#include <Wire.h>
#include <Canbus.h>

// Parameters
#define DEBUG           1
#define DISPLAY_ADDR    0x71
#define RPM_LEVEL_0     1000
#define RPM_LEVEL_1     2000
#define RPM_LEVEL_2     2500
#define RPM_LEVEL_3     3000
#define RPM_LEVEL_4     3500
#define RPM_LEVEL_5     5000
#define RPM_LEVEL_6     6000
#define RPM_LEVEL_7     7000

// Pins
#define LED_0           6
#define LED_1           5
#define LED_2           4
#define LED_3           3
#define LED_4           A0
#define LED_5           A1
#define LED_6           A2
#define LED_7           A3

// Global variables
char buf[512];
uint32_t mph;
uint32_t rpm;
uint8_t g_i;
uint8_t g_len;

void setup() {
  
#if DEBUG
  Serial.begin(9600);
  Serial.println("CarHUD");
#endif
  
  // Configure 7-seg display, reset display
  Wire.begin();
  Wire.beginTransmission(DISPLAY_ADDR);
  Wire.write('v');
  Wire.endTransmission();
  
  // Configure LEDs
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  pinMode(LED_7, OUTPUT);
  
  // Configure CAN bus shield
  if ( Canbus.init(CANSPEED_500) ) {
#if DEBUG
    Serial.println("CAN init OK");
#endif
  } else {
#if DEBUG
    Serial.println("Error on CAN init");
#endif
    while(1);
  }
  
  // Initialize MPH and RPM
  mph = 0;
  rpm = 0;
  
  delay(1000);
}

void loop() {
  
  // Read RPM
  if ( Canbus.ecu_req(ENGINE_RPM, buf) == 1 ) {
    g_len = strlen(buf);
    if ( g_len > 4 ) {
      g_len = 4;
    }
    rpm = 0;
    for ( g_i = 0; g_i < g_len; g_i++ ) {
      rpm += (buf[g_i] - '0') * pow(10, (g_len - 1 - g_i));
    }
  }
  
  // Read MPH
  if ( Canbus.ecu_req(VEHICLE_SPEED, buf) == 1 ) {
    g_len = strlen(buf);
    if ( g_len > 3 ) {
      g_len = 3;
    }
    rpm = 0;
    for ( g_i = 0; g_i < g_len; g_i++ ) {
      rpm += (buf[g_i] - '0') * pow(10, (g_len - 1 - g_i));
    }
  }
  
  // Update 7-segment and LED bar
#if DEBUG
  Serial.print("MPH: ");
  Serial.print(mph);
  Serial.print(" RPM: ");
  Serial.println(rpm);
#endif
  sevSegSend(mph);
  lightEmUp(rpm);
  
  delay(10);
  
}

// LED bar control
void lightEmUp(uint32_t val) {
  
  if ( val >= RPM_LEVEL_0 ) {
    digitalWrite(LED_0, LOW);
  } else {
    digitalWrite(LED_0, HIGH);
  }
  
  if ( val >= RPM_LEVEL_1 ) {
    digitalWrite(LED_1, LOW);
  } else {
    digitalWrite(LED_1, HIGH);
  }
  
  if ( val >= RPM_LEVEL_2 ) {
    digitalWrite(LED_2, LOW);
  } else {
    digitalWrite(LED_2, HIGH);
  }
  
  if ( val >= RPM_LEVEL_3 ) {
    digitalWrite(LED_3, LOW);
  } else {
    digitalWrite(LED_3, HIGH);
  }
  
  if ( val >= RPM_LEVEL_4 ) {
    digitalWrite(LED_4, LOW);
  } else {
    digitalWrite(LED_4, HIGH);
  }
  
  if ( val >= RPM_LEVEL_5 ) {
    digitalWrite(LED_5, LOW);
  } else {
    digitalWrite(LED_5, HIGH);
  }
  
  if ( val >= RPM_LEVEL_6 ) {
    digitalWrite(LED_6, LOW);
  } else {
    digitalWrite(LED_6, HIGH);
  }
  
  if ( val >= RPM_LEVEL_7 ) {
    digitalWrite(LED_7, LOW);
  } else {
    digitalWrite(LED_7, HIGH);
  }
}

// 7-Segment Display Fucntion
void sevSegSend(uint32_t val) {
  Wire.beginTransmission(DISPLAY_ADDR);
  Wire.write(val / 1000);
  val %= 1000;
  Wire.write(val / 100);
  val %= 100;
  Wire.write(val / 10);
  val %= 10;
  Wire.write(val);
  Wire.endTransmission();
}