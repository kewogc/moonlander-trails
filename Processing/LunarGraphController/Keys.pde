
void keyPressed() {
  if (key == 'r') { 
    sendSerial("reset");
  }
  if (key == 'f') { 
    //resize(displayWidth, displayHeight);  
    //frame.setSize(displayWidth, displayHeight);
    // frame.setUndecorated(true); 
    //frame.setLocation(0, 0);
    fullSizePreview = !fullSizePreview; 
    if(fullSizePreview) {
      //resize(displayWidth, displayHeight);  
      //frame.setSize(displayWidth, displayHeight);
      //frame.setLocation(0, 0);
      noSmooth();
    } 
    else {
      //resize(round(viewWidth*viewScale), round(viewHeight*viewScale));  
      //frame.setSize(round(viewWidth*viewScale), round(viewHeight*viewScale));
      smooth();
    } 
  }
  if(key == 'l') { 
    drawLandscape(); 
  }
}

