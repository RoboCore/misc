
#include "sith.h"

// ------------------------

void finishwhatVaderStarted(void){
  Serial.println("Welcome to the Dark Side!");
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  pinMode(5, OUTPUT);
  
  while(1){
    for(byte i=0 ; i < 255 ; i++){
      analogWrite(5,i);
      delay(5);
    }
    delay(30);
    for(byte i=255 ; i > 0 ; i--){
      analogWrite(5,i);
      delay(5);
    }
  }
}

// ------------------------

void DarkSide::invitation(boolean b){
  Serial.println("Hello master, I am your servant");
}

// ------------------------

DarkSide KyloRen;



