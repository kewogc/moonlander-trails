#include <AFMotor.h>

#include <AccelStepper.h>


#include "constants.h"
#include "DynamicMotor.h"
#include "Motors.h"
#include "Button.h"

#include "TimerManager.h"

TimerManager timerManager; 

unsigned int state = STATE_WAITING; 
unsigned int calibrationProgressA = 0; 
unsigned int calibrationProgressB = 0; 


float maxJogSpeed = 2000.0f; 

Button jogUpButtonA   = Button(A_JOG_UP_PIN); 
Button jogDownButtonA = Button(A_JOG_DOWN_PIN); 
Button jogUpButtonB   = Button(B_JOG_UP_PIN); 
Button jogDownButtonB = Button(B_JOG_DOWN_PIN); 
Button resetButton    = Button(RESET_BUTTON_PIN);

Button endStopMinButtonA = Button(A_END_STOP_MIN_PIN); 
Button endStopMaxButtonA = Button(A_END_STOP_MAX_PIN); 
Button calibrationButtonA = Button(A_CALIBRATION_PIN);

#ifdef USE_TEST_MACHINE

Button endStopMinButtonB = Button(B_END_STOP_MIN_PIN); 
Button endStopMaxButtonB = Button(B_END_STOP_MAX_PIN); 
Button calibrationButtonB = Button(B_CALIBRATION_PIN); 

#else 

Button endStopMinButtonB = Button(B_END_STOP_MIN_PIN, false); 
Button endStopMaxButtonB = Button(B_END_STOP_MAX_PIN, false); 
Button calibrationButtonB = Button(B_CALIBRATION_PIN, false); 

#endif


// DON'T FORGET to add to NUM_BUTTONS if you add to the array
Button* buttons[] = { 
  &jogUpButtonA, 
  &jogDownButtonA, 
  &jogUpButtonB, 
  &jogDownButtonB, 
  &endStopMinButtonA, 
  &endStopMaxButtonA, 
  &endStopMinButtonB, 
  &endStopMaxButtonB, 
  &calibrationButtonA, 
  &calibrationButtonB,
  &resetButton
}; 
const int NUM_BUTTONS = 11; 

void setup()  { 

  Serial.begin(9600); 

  initMotors(); 
  initEndStopInterrupts(); 


}

void loop() { 
  // mechanism for doing regular updates no more than 100 times a second. 
  timerManager.update();
  updateButtons(); 

  if(state!=STATE_RESET) checkEndStops(); 

  if(state == STATE_EMERGENCY_STOP) { 

    if(resetButton.isPressed()) { 
      changeState(STATE_RESET); 
    }

  } 
  else if(state == STATE_RESET) { 

    if(clearEndStops()) { 
      changeState(STATE_CALIBRATING); 

    }; 

  } 
  else if(state == STATE_CALIBRATING) { 


    // if we haven't got both motors loose enough to unset the switches, 
    // move them both down until they are
    if((calibrationProgressA == 0) || (calibrationProgressB == 0)) {
      if((calibrationProgressA == 0) && (calibrationButtonA.isPressed())) {
        motorA.setSpeed(5000); 
      } 
      else { 
        motorA.stop(); 
        calibrationProgressA = 1; 
      }

      if((calibrationProgressB == 0) && (calibrationButtonB.isPressed())) {
        motorB.setSpeed(5000); 
      } 
      else { 
        motorB.stop(); 
        calibrationProgressB = 1;

      }
    }

    if(calibrationProgressA == 1) { 
      if(calibrationButtonA.isPressed()) { 
        motorA.stop(); 
        calibrationProgressA = 2;   
      } 
      else { 
        motorA.setSpeed(-5000); 
      }
    }

    if(calibrationProgressB == 1) { 
      if(calibrationButtonB.isPressed()) { 
        motorB.stop(); 
        calibrationProgressB = 2;   
      } 
      else { 
        motorB.setSpeed(-5000); 
      }
    }

    if(calibrationProgressA == 2) { 
      if(calibrationButtonA.isPressed()) { 
        motorA.setSpeed(500);    
      } 
      else { 
        // CALIBRATION POINT IS HERE!
        
        Serial.print("MotorA calibration point : "); 
        Serial.println(motorA.accelStepper.currentPosition()); 
        
        motorA.stop(); 
        calibrationProgressA = 3; 
      }
    }

    if(calibrationProgressB == 2) { 
      if(calibrationButtonB.isPressed()) { 
        motorB.setSpeed(500);  
      } 
      else { 
        // CALIBRATION POINT IS HERE!
        Serial.print("MotorB calibration point : "); 
        Serial.println(motorB.accelStepper.currentPosition()); 
        motorB.stop(); 
        calibrationProgressB = 3; 
      }
    }

    if((calibrationProgressA ==3) && ( calibrationProgressB ==3)) { 
      changeState(STATE_WAITING);       
    }

  } 
  else if(state == STATE_WAITING) { 

    if(resetButton.isPressed()) { 
      changeState(STATE_CALIBRATING); 
    }
    if(timerManager.do100msUpdate) updateJogButtons(); 
  }


  motorA.update(timerManager.do100msUpdate); 
  motorB.update(timerManager.do100msUpdate); 

  if(timerManager.do100msUpdate) { 
    //    
    //    Serial.println(state);
    //    Serial.print(" "); 
    //    Serial.println(digitalRead(B_END_STOP_MAX_PIN)); 

    //    Serial.print(motorB.currentSpeed); 
    //    Serial.print(" "); 
    //    Serial.print(motorB.accelStepper.speed()); 
    //    Serial.print(" "); 
    //    Serial.println(jogUpButtonB.isPressed()); 
    //    //Serial.println(digitalRead(B_JOG_UP_PIN));

    // Serial.println(jogUpButtonB.isPressed()); 


  }

}

void checkEndStops() { 

  if( (endStopMinButtonA.isPressed()) ||
    (endStopMaxButtonA.isPressed()) ||
    (endStopMinButtonB.isPressed()) ||
    (endStopMaxButtonB.isPressed()) ) {

    if(changeState(STATE_EMERGENCY_STOP)){  

      Serial.print(endStopMinButtonA.isPressed()) ;
      Serial.print(" "); 
      Serial.print(endStopMaxButtonA.isPressed()) ;
      Serial.print(" "); 
      Serial.print(endStopMinButtonB.isPressed()) ;
      Serial.print(" "); 
      Serial.println(endStopMaxButtonB.isPressed()) ;
    }
  }

}

boolean clearEndStops() { 


  boolean endStopsClear = true; 

  if(endStopMinButtonA.isPressed()) {

    motorA.setSpeed(500); 
    endStopsClear = false; 
  } 
  else if(endStopMaxButtonA.isPressed()) {

    motorA.setSpeed(-500); 
    endStopsClear = false; 
  } 
  else { 
    motorA.stop();  

  }

  if(endStopMinButtonB.isPressed()) {

    motorB.setSpeed(500); 
    endStopsClear = false; 
  } 
  else if(endStopMaxButtonB.isPressed()) {

    motorB.setSpeed(-500); 
    endStopsClear = false; 
  } 
  else { 
    motorB.stop();  

  }

  return endStopsClear; 


}

boolean changeState(int newState) { 
  if(state == newState) return false; 

  state = newState; 
  motorA.stop(); 
  motorB.stop(); 


  if(state == STATE_CALIBRATING) { 

    if(calibrationButtonA.isPressed()) { 
      calibrationProgressA = 0; 
    } 
    else { 
      calibrationProgressA = 1; 
    }

    if(calibrationButtonB.isPressed()) { 
      calibrationProgressB = 0; 
    } 
    else { 
      calibrationProgressB = 1; 
    }



  }  

  Serial.print("CHANGE STATE : ");   
  Serial.println(state);
  //    Serial.print(" "); 
  //    Serial.println(digitalRead(B_END_STOP_MAX_PIN)); 
  return true; 
}


void updateButtons() {  

  for(int i = 0; i< NUM_BUTTONS; i++) { 
    buttons[i]->update(); 
  }

}


void updateJogButtons() { 

  
  if(jogUpButtonA.isPressed()) { 

    //jogSpeedA = mapEaseInOut((float)jogUpButtonA.getTimeSinceChange(), 0.0f, 2000.0f, 0.0f, -maxJogSpeed);
    motorA.setSpeedDirect(mapEaseInOut((float)jogUpButtonA.getTimeSinceChange(), 0.0f, 2000.0f, 0.0f, -maxJogSpeed)); 

  } 
  else if(jogDownButtonA.isPressed()) { 
    motorA.setSpeedDirect(mapEaseInOut((float)jogDownButtonA.getTimeSinceChange(), 0.0f, 2000.0f, 0.0f, maxJogSpeed)); 
  } 
  else { 

    motorA.stop(); 

  }


  if(jogUpButtonB.isPressed()) { 
    motorB.setSpeedDirect(mapEaseInOut((float)jogUpButtonB.getTimeSinceChange(), 0.0f, 2000.0f, 0.0f, -maxJogSpeed)); 
  } 
  else if(jogDownButtonB.isPressed()) { 
    motorB.setSpeedDirect(mapEaseInOut((float)jogDownButtonB.getTimeSinceChange(), 0.0f, 2000.0f, 0.0f, maxJogSpeed)); 
  } 
  else { 
    motorB.stop(); 
  }



  if(timerManager.do100msUpdate) { 
    // trace stuff out here if necessary
  }

}

void initEndStopInterrupts() { 
  //  attachInterrupt(A_END_STOP_MIN_INTERRUPT, endStopMinTriggered, FALLING); 
  //  digitalWrite(A_END_STOP_MIN_PIN, HIGH); 
  //  attachInterrupt(A_END_STOP_MAX_INTERRUPT, endStopMinTriggered, FALLING); 
  //  digitalWrite(A_END_STOP_MAX_PIN, HIGH); 
  // 
  //  pinMode(B_END_STOP_MAX_PIN, INPUT); 
  //  digitalWrite(B_END_STOP_MAX_PIN, HIGH); 

  //attachInterrupt(B_END_STOP_MAX_INTERRUPT, endStopMinTriggered, FALLING); 


}

void endStopMinTriggered() { 

  state = STATE_EMERGENCY_STOP;  

}





