#include <Arduino.h>
#include<TM1637.h>

// put function declarations here:
#define LedRed 4
#define LedBlue 2
#define Key 12

uint cnt = 0;

byte readButton();

void setup() {
  // put your setup code here, to run once:
  pinMode(LedRed, OUTPUT);
  pinMode(LedBlue, OUTPUT);
  pinMode(Key, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Hello, ESP3eee2!");
  delay(10);
}

void loop() {
  
  if(readButton() == 12) {
    cnt ++;
    if( cnt == 1 ){
      digitalWrite(LedRed, HIGH);
      digitalWrite(LedBlue, LOW);
    }
    else if( cnt == 2 ){
      digitalWrite(LedRed, LOW);
      digitalWrite(LedBlue, HIGH);
      cnt = 0;
    }
  }
  delay(100);
}

byte readButton() {
  while (true) {
    if (digitalRead(Key) == LOW) {
      return 12;
    }
  }
  delay(1);
}
