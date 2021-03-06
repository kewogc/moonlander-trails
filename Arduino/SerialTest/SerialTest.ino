
char incoming[128]; 
char parseBuffer[15]; 
int cmdNumber; 
int cmd; 
float p1; 
float p2; 

int incomingCharCount = 0; 
int incomingParsePos = 0; 


void setup() { 


  Serial.begin(115200); 
  Serial.println("RESTART"); 
  Serial.println("ready:50"); 



}


void loop() { 

  checkIncoming(); 


}



void checkIncoming() { 

  if(Serial.available()>0) { 

    char c = Serial.read(); 
    incoming[incomingCharCount] = c; 

    incomingCharCount++; 

    if(c=='\0') {

      // format = <cmdnum>,<cmd>,<p1>,<p2> 

      incomingParsePos = 0; 
      
      
      cmdNumber = parseIncomingInt(); 
      cmd = parseIncomingInt(); 
      p1  = parseIncomingFloat(); 
      p2  = parseIncomingFloat(); 

      Serial.print("cn:"); 
      Serial.print(cmdNumber);       
      Serial.print(" c:"); 
      Serial.print(cmd); 
      Serial.print(" p1:"); 
      Serial.print(p1); 
      Serial.print(" p2:");
      Serial.println(p2);  
    
      incoming[0] = '\0'; 
      incomingCharCount = 0; 
      Serial.println("ready:1"); 
    } 

  }
}

int parseIncomingInt() { 

  insertNextNumberIntoBuffer(); 
  return atoi(parseBuffer); 

}


float parseIncomingFloat() { 

  insertNextNumberIntoBuffer(); 
  return atof(parseBuffer); 

}

boolean insertNextNumberIntoBuffer() { 

  
  // should probably check for end of parsebuffer or incoming string
  
  if(incoming[incomingParsePos]==',') incomingParsePos++;  

  int j = 0; 

  while(incoming[incomingParsePos]>='.' & incoming[incomingParsePos]<='9'){ 

    parseBuffer[j] = incoming[incomingParsePos]; 
    incomingParsePos++;
    j++;
  }
  parseBuffer[j] = '\0'; 
  return true; 

}




