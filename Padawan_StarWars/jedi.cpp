
#include "jedi.h"

// ------------------------

const int *pin7seg[7] = { &pinLED1 , &pinLED2 , &pinLED3 , &pinLED4 , &pinRGB_R , &pinRGB_G , &pinRGB_B };

byte idle = 0;
byte awaken = 1;

char forceSide = dark;
boolean force = false;
byte F;

float value_LM35;
int value_LDR;
int value_LDR_start;
float temperature;

// ------------------------

void forceStatus(byte b){
  Serial.begin(9600);
  Serial.println("Status: idle");
  
  pinMode(pinLM35, INPUT);
  pinMode(pinLDR, INPUT);
  pinMode(pinPot, INPUT);
  pinMode(pinBtn, INPUT);
  pinMode(pinSelect, OUTPUT);
  pinMode(pinRelay, OUTPUT);
  for(int i=DISPLAY_A ; i <= DISPLAY_G ; i++){
    pinMode(*pin7seg[i], OUTPUT);
  }
  
  digitalWrite(pinSelect, LOW);
  digitalWrite(pinRelay, LOW);
  for(int i=DISPLAY_A ; i <= DISPLAY_G ; i++){
    digitalWrite(*pin7seg[i], LOW);
  }
  
  analogReference(ANALOG_REFERENCE); // use 1.1V
}

// ------------------------

void forceMode(byte b){
  Serial.println("Force awaken!");
  
  delay(1000); // give a moment before reading the ambient light
  
  // get LDR first reading
  value_LDR_start = 0;
  for(int i=0 ; i < 10 ; i++){ // watch for variable overflow
    value_LDR_start += analogRead(pinLDR);
    delay(10);
  }
  value_LDR_start /= 10;
  value_LDR_start -= 200; // decrease a little to avoid oscillations
  if(value_LDR_start < 0)
    value_LDR_start = 30;
}

// ------------------------

char userRead(void){
  Serial.println("Waiting for user input");
  
  // wait for user hover
  value_LDR = 1023; // reset
  byte current_segment = DISPLAY_A;
  unsigned long next_time = 0;
  digitalWrite(pinSelect, SELECT_DISPLAY);
  
  int count = 10;
  float value1 = 0, value2 = 0;
  boolean locked = true;
  
  while(1){
//    value_LDR = analogRead(pinLDR);
    value_LDR = 0.6 * value_LDR + 0.4 * analogRead(pinLDR);
    delay(5);
    
    if(value_LDR >= value_LDR_start){
      count = 10; // reset
      
      if(millis() > next_time){
        current_segment++;
        if(current_segment >= DISPLAY_G)
          current_segment = DISPLAY_A;
        _SetDisplay(1 << current_segment);
        next_time = millis() + 100;
      }
    } else {
      if(millis() > next_time){
        count--;
        if(count < 0)
          break;
        next_time = millis() + 1000;
      }
      
      // update counter
      switch(count){
        case 9:
          _readLM35();
          value1 = value_LM35;
          _SetDisplay(ALPHANUMERIC_9);
          break;
        case 8: _SetDisplay(ALPHANUMERIC_8); break;
        case 7: _SetDisplay(ALPHANUMERIC_7); break;
        case 6: _SetDisplay(ALPHANUMERIC_6); break;
        case 5: _SetDisplay(ALPHANUMERIC_5); break;
        case 4: _SetDisplay(ALPHANUMERIC_4); break;
        case 3: _SetDisplay(ALPHANUMERIC_3); break;
        case 2: _SetDisplay(ALPHANUMERIC_2); break;
        case 1: _SetDisplay(ALPHANUMERIC_1); break;
        case 0:
          _readLM35();
          value2 = value_LM35;
          _SetDisplay(ALPHANUMERIC_0);
          break;
      }
    }
  }
  
  // animate calculation
  digitalWrite(pinSelect, LOW);
  for(int i=DISPLAY_A ; i <= DISPLAY_G ; i++){
    digitalWrite(*pin7seg[i], LOW);
  }
  count = 0;
  byte state = LOW;
  digitalWrite(pinSelect, SELECT_LED);
  unsigned long stop_time = millis() + 4000;
  while(millis() <= stop_time){
    count++;
    if(count > 1)
      digitalWrite(pinLED1, state);
    if(count > 2)
      digitalWrite(pinLED2, state);
    if(count > 3)
      digitalWrite(pinLED3, state);
    if(count > 4){
      digitalWrite(pinLED4, state);
      count = 0;
      state = (state == LOW) ? HIGH : LOW;
    }
    delay(100);
  }
  for(int i=DISPLAY_A ; i <= DISPLAY_G ; i++){
    digitalWrite(*pin7seg[i], LOW);
  }
  
  // calculate
  float res = value2 - value1;
  force = true;
  if(res > 10)
    return light;
  else
    return dark;
}

// ------------------------

byte LightSide(void){
  Serial.println("Welcome to the Light Side!");
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  pinMode(6, OUTPUT);
  
  while(1){
    for(byte i=0 ; i < 255 ; i++){
      analogWrite(6,i);
      delay(5);
    }
    delay(30);
    for(byte i=255 ; i > 0 ; i--){
      analogWrite(6,i);
      delay(5);
    }
  }
}

// ------------------------
// ------------------------

// New AnalogRead function for board compatibility
int _newAnalogRead(int pin){
  float value = analogRead(pin) * CONVERSION_FACTOR_BOARD;
  if(value > 1023)
    value = 1023;
  return (int)value;
}

// ------------------------

void _readLM35(void){
  value_LM35 = FILTER_FACTOR_LM35 * value_LM35 + (1.0 - FILTER_FACTOR_LM35)*analogRead(A0);
  temperature = value_LM35 * CONVERSION_FACTOR_LM35;
}

// ------------------------

void _SetDisplay(byte character){
#ifdef SOFTWARE_LED_RESET
  _ResetPins();
#endif
  
  digitalWrite(pinSelect, SELECT_DISPLAY);
  
  byte mask;
  for(int i=DISPLAY_A ; i <= DISPLAY_G ; i++){
    mask = (1 << i);
    _SetPin(pin7seg[i], (character & mask));
  }
}

// ------------------------

// Set a pin state
void _SetPin(const int *pin, byte state){
  // check for null pointer
  if(pin == NULL)
    return;
  
  if((state == HIGH) || (state != 0)){
    digitalWrite(*pin, HIGH);
  } else {
    digitalWrite(*pin, LOW);
  }
}

// ------------------------










