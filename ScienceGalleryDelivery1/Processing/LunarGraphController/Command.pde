final int COMMAND_MOVE = 0; 
final int COMMAND_DRAW = 1; 
final int COMMAND_DRAW_DIRECT = 2; 
final int COMMAND_RESTART = 3; 
final int COMMAND_FINISH = 4; 


class Command { 
  
//  
//  Command(){
//     Command(0,0,0);
//  }
//  
  Command (int _c, float _p1, float _p2) { 
    
    c = _c; 
    p1 = _p1; 
    p2 = _p2; 
    
    //p3 = _p3; 
    //p4 = _p4;   
  }
  
  
  
  int c; 
  float p1, p2;
  
  
  
};
