
DynamicMotor motorA; 
DynamicMotor motorB; 


#ifdef USE_TEST_MACHINE

AF_Stepper stepperA = AF_Stepper(800,1); 
AF_Stepper stepperB = AF_Stepper(800,2); 



void forwardA() { 
  stepperA.onestep(FORWARD, INTERLEAVE); 
}

void backwardA() { 
 stepperA.onestep(BACKWARD, INTERLEAVE); 
}

void forwardB() { 
 stepperB.onestep(FORWARD, INTERLEAVE); 
}

void backwardB() { 
  stepperB.onestep(BACKWARD, INTERLEAVE); 
}



#else 


#endif

void initMotors() { 
  
  
#ifdef USE_TEST_MACHINE  
forwardA(); 
forwardB(); 
  motorA.initAFStepper(&stepperA,forwardA, backwardA); 
  motorB.initAFStepper(&stepperB,forwardB, backwardB); 

#else 

  motorA.initGecko(A_STEP_PIN, A_DIR_PIN); 
  motorB.initGecko(B_STEP_PIN, B_DIR_PIN); 
  

  motorB.accelStepper.setPinsInverted(true, false); 
  pinMode(A_ERR_RESET_PIN, OUTPUT); 
  pinMode(B_ERR_RESET_PIN, OUTPUT); 
  
  digitalWrite(A_ERR_RESET_PIN, HIGH); 
  digitalWrite(B_ERR_RESET_PIN, HIGH); 
  
  delay(2000); 
  
  
  digitalWrite(A_ERR_RESET_PIN, LOW); 
  digitalWrite(B_ERR_RESET_PIN, LOW); 
  
  pinMode(A_ERR_RESET_PIN, INPUT); 
  pinMode(B_ERR_RESET_PIN, INPUT); 
  
    
#endif
//
//
//  motorA.setMaxSpeed(5000); 
//  motorB.setMaxSpeed(5000); 
//  motorA.setAcceleration(1000); 
//  motorB.setAcceleration(1000); 
//  motorA.move(1); 
//  motorB.move(1); 
//  motorA.runToPosition(); 
//  motorB.runToPosition(); 




}


