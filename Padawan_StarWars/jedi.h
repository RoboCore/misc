#ifndef JEDI_H
#define JEDI_H

#include <Arduino.h>

// ------------------------

// Conversion Factor for the LM35 sensor
//  (sensor has 10mV/oC resolution)
#if defined(__AVR_ATmega328P__)          // UNO
#define CONVERSION_FACTOR_LM35   0.1075269
#define CONVERSION_FACTOR_BOARD  1
#define ANALOG_REFERENCE         INTERNAL
#elif defined (__AVR_ATmega2560__)       // Mega 2560
#define CONVERSION_FACTOR_LM35   0.1075269
#define CONVERSION_FACTOR_BOARD  1
#define ANALOG_REFERENCE         INTERNAL1V1
#elif defined (__AVR_ATmega32U4__)       // Leonardo
#define CONVERSION_FACTOR_LM35   0.2502444
#define CONVERSION_FACTOR_BOARD  2.3272727
#define ANALOG_REFERENCE         INTERNAL // 2.56V
#else                                    // other (not supported)
#define CONVERSION_FACTOR_LM35   1
#define CONVERSION_FACTOR_BOARD  1
#define ANALOG_REFERENCE         DEFAULT
#endif

#define DEBOUNCE 25  // [ms]
#define FILTER_FACTOR_LM35 0 // 0 <= k <= 1

// ------------------------

#define dark   '0'
#define light  '1'

#define ma LightSide()

// ------------------------
// PINS *****

const int pinLM35 = A0;
const int pinLDR = A1;
const int pinPot = A2;
const int pinBtn = 2;

#define SELECT_LED      HIGH
#define SELECT_DISPLAY   LOW
const int pinSelect = A3;

const int pinRelay = 9;

// LED RGB
const int pinRGB_R = 5;
const int pinRGB_G = 3;
const int pinRGB_B = 6;

// LED array
const int pinLED1 = A4;
const int pinLED2 = A5;
const int pinLED3 = 7;
const int pinLED4 = 8;

// 7 segments display
#define DISPLAY_A 0
#define DISPLAY_B 1
#define DISPLAY_C 2
#define DISPLAY_D 3
#define DISPLAY_E 4
#define DISPLAY_F 5
#define DISPLAY_G 6

// ------------------------
// 7 segments display *****

#define ALPHANUMERIC_NONE 0x00

#define ALPHANUMERIC_0 ((1 << DISPLAY_A) | (1 << DISPLAY_B) | (1 << DISPLAY_C) | (1 << DISPLAY_D) | (1 << DISPLAY_E) | (1 << DISPLAY_F))
#define ALPHANUMERIC_1 ((1 << DISPLAY_B) | (1 << DISPLAY_C))
#define ALPHANUMERIC_2 ((1 << DISPLAY_A) | (1 << DISPLAY_B) | (1 << DISPLAY_D) | (1 << DISPLAY_E) | (1 << DISPLAY_G))
#define ALPHANUMERIC_3 ((1 << DISPLAY_A) | (1 << DISPLAY_B) | (1 << DISPLAY_C) | (1 << DISPLAY_D) | (1 << DISPLAY_G))
#define ALPHANUMERIC_4 ((1 << DISPLAY_B) | (1 << DISPLAY_C) | (1 << DISPLAY_F) | (1 << DISPLAY_G))
#define ALPHANUMERIC_5 ((1 << DISPLAY_A) | (1 << DISPLAY_C) | (1 << DISPLAY_D) | (1 << DISPLAY_F) | (1 << DISPLAY_G))
#define ALPHANUMERIC_6 ((1 << DISPLAY_A) | (1 << DISPLAY_C) | (1 << DISPLAY_D) | (1 << DISPLAY_E) | (1 << DISPLAY_F) | (1 << DISPLAY_G))
#define ALPHANUMERIC_7 ((1 << DISPLAY_A) | (1 << DISPLAY_B) | (1 << DISPLAY_C))
#define ALPHANUMERIC_8 ((1 << DISPLAY_A) | (1 << DISPLAY_B) | (1 << DISPLAY_C) | (1 << DISPLAY_D) | (1 << DISPLAY_E) | (1 << DISPLAY_F) | (1 << DISPLAY_G))
#define ALPHANUMERIC_9 ((1 << DISPLAY_A) | (1 << DISPLAY_B) | (1 << DISPLAY_C) | (1 << DISPLAY_D) | (1 << DISPLAY_F) | (1 << DISPLAY_G))

// ------------------------
// Variables *****

extern byte idle;
extern byte awaken;
extern char forceSide;
extern boolean force;
extern byte F;

extern float value_LM35;
extern int value_LDR;
extern int value_LDR_start;
extern float temperature;

// ------------------------
// Prototypes *****

void forceStatus(byte b);
void forceMode(byte b);

char userRead(void);

byte LightSide(void);

// ------------------------
// Prototypes (private) *****

int _newAnalogRead(int);
void _readLM35(void);
void _SetDisplay(byte);
void _SetPin(const int *, byte);

// ------------------------

#endif // JEDI_H


