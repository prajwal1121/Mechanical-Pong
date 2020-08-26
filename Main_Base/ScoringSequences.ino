void scoreRight(int colorselection, int score){
  for(int i = 86; i > 86 -(score*2); i= i-2) {  
    if (colorSelectionRight == 0){
        borderLED[i] = CRGB::Blue;    
    }
            
    if (colorSelectionRight == 1){
        borderLED[i] = CRGB::Red;    
    }
    
    if (colorSelectionRight == 2){
        borderLED[i] = CRGB::Green;    
    }
    
    if (colorSelectionRight == 3){
        borderLED[i] = CRGB::Magenta;    
    }
    
    if (colorSelectionRight == 4){
        borderLED[i] = CRGB::Cyan;    
    }
    
    if (colorSelectionRight == 5){
        borderLED[i] = CRGB::Gold;    
    }
    FastLED.show();
  }
}

void scoreLeft(int colorselection, int score){
  for(int i = 5; i < 5+(score*2); i+=2) {  
    if (colorSelectionLeft == 0){
        borderLED[i] = CRGB::Blue;    
    }
            
    if (colorSelectionLeft == 1){
        borderLED[i] = CRGB::Red;    
    }
    
    if (colorSelectionLeft == 2){
        borderLED[i] = CRGB::Green;    
    }
    
    if (colorSelectionLeft == 3){
        borderLED[i] = CRGB::Magenta;    
    }
    
    if (colorSelectionLeft == 4){
        borderLED[i] = CRGB::Cyan;    
    }
    
    if (colorSelectionLeft == 5){
        borderLED[i] = CRGB::Gold;    
    }
    FastLED.show();
  }
}
