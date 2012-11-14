
#include "KeyButton.h"

const int THRUST_PIN = A0; 
const int THRUST_HIGH = A1; 
const int THRUST_LOW = 3; 

const int LEFT_PIN = 2; // light grey
const int RIGHT_PIN = 9; // orange
const int START_PIN = 8;
const int SELECT_PIN = 11;
const int ABORT_PIN = 7; 

KeyButton leftButton   = KeyButton(LEFT_PIN, 'l'); 
//KeyButton rightButton   = KeyButton(RIGHT_PIN, 'r', false); 
//KeyButton startButton   = KeyButton(START_PIN, 't'); 
//KeyButton selectButton   = KeyButton(SELECT_PIN, 'e' ); 
//KeyButton abortButton   = KeyButton(ABORT_PIN, 'b'); 

KeyButton* buttons[] = { 
  &leftButton, 
  //&rightButton, 
  //&startButton, 
  //&selectButton, 
  //  &abortButton
}; 
const int NUM_BUTTONS = 1; 

float topValue = 0; 
float bottomValue = 1024; 
float currentReading; 
float smoothReading; 

int currentKey = 0; 
int counter =0; 

void setup() { 

  // LOW and HIGH for thrust control
  pinMode(THRUST_LOW, OUTPUT); 
  pinMode(THRUST_HIGH, OUTPUT); 
  digitalWrite(THRUST_LOW, LOW); 
  digitalWrite(THRUST_HIGH, HIGH); 

  pinMode(LEFT_PIN, INPUT); 
  pinMode(RIGHT_PIN, INPUT); 
  pinMode(START_PIN, INPUT); 
  pinMode(SELECT_PIN, INPUT); 
  pinMode(ABORT_PIN, INPUT); 



  Serial.begin(38400); 
  Keyboard.begin(); 
  Mouse.end();

  updateButtons(); 

}

void loop() { 
  updateButtons();


  currentReading = 10;//analogRead(THRUST_PIN);

  if(topValue<currentReading) topValue = currentReading; 
  if(bottomValue>currentReading) bottomValue = currentReading; 

  smoothReading += ((currentReading-smoothReading)*0.1f);
  int newKey;
  if(abs(smoothReading - bottomValue)<10) newKey = 0;
  else newKey = round(map(smoothReading, bottomValue, topValue, 1, 10));
  if(newKey<0) newKey = 0; 
  if(newKey>9) newKey = 9; 



  if(newKey!=currentKey) {

    currentKey = newKey; 
    pressKey(48+currentKey); 
    Serial.println(smoothReading);
    Keyboard.releaseAll(); 
    counter = 0; 
  } 
  else if (counter>5) {
    Keyboard.releaseAll(); 

  }

  counter++; 

  //Mouse.move(0,analogRead(thrustPin));
  delay(10);  

}

void pressKey(int numkey) { 
  //Keyboard.press(numkey); 
  Serial.println(char(numkey));  


}


void updateButtons() {  
  boolean changed = false; 
  for(int i = 0; i< NUM_BUTTONS; i++) { 
    if( buttons[i]->update() ) changed = true;  
  }

  if(changed) { 
    // send button data...  
    Serial.print("buttons:"); 
    for(int i = 0; i< NUM_BUTTONS; i++) { 
      Serial.print(buttons[i]->isOn()); 
    }
    Serial.println(""); 
  }
}


