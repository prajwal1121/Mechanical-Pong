void PONG(){
  switch (machineState) {
    case homing: { //Homing sequence
      //Turn all the LEDs red
      for(int i = 0; i < NUM_LEDS_PAD; i++) {
        leftPadLED[i] = CRGB::Red;    
      }
      for(int i = 0; i < NUM_LEDS_PAD; i++) {
        rightPadLED[i] = CRGB::Red;    
      }
      for(int i = 0; i < NUM_LEDS_BALL; i++) {
        ballLED[i] = CRGB::Red;    
      }
      for(int i = 0; i < NUM_LEDS_BORDER; i++) {
        borderLED[i] = CRGB::Red;    
      }
      FastLED.show();
      
      StepControl ballSteppers; 
      long homingPositions[2]; 

      //Move down until the z-min endstop is hit
      homingPositions[0] = -5000000;
      homingPositions[1] = 5000000;
      LBall.setTargetAbs(homingPositions[0]);
      RBall.setTargetAbs(homingPositions[1]);
            
      while(!digitalRead(ballZminStop)){
        ballSteppers.moveAsync(LBall,RBall);
        delay(10);
      }

      //Move right until the x-max endstop is hit
      homingPositions[0] = -5000000;
      homingPositions[1] = -5000000;
      LBall.setTargetAbs(homingPositions[0]);
      RBall.setTargetAbs(homingPositions[1]);
      
      while(!digitalRead(ballXmaxStop)){
        //ballSteppers.run();
        ballSteppers.moveAsync(LBall,RBall);
        delay(10);
      }
      
      if (digitalRead(ballXmaxStop) and digitalRead(ballZminStop)){
        RBall.setPosition(ball_horizontal_max);  
        LBall.setPosition(ball_horizontal_max); 
      }

      //Set known position
      currentBallPosX = ball_horizontal_max;  
      currentBallPosY = 0;

      machineState = restore;
      break;      
    }
    case colorSelect: { //Allow players to select paddle color      
      leftPad.setTargetAbs(int(paddlemax/2));
      rightPad.setTargetAbs(int(paddlemax/2));

      //Move left paddle up
      if (!leftPadCentered){
        StepControl paddleSteppers;
        paddleSteppers.move(leftPad);
        leftPadCentered = true;
        lpadtarget = int(paddlemax/2);
      }

      //Use left nunchuck to scroll thru color choices
      if (leftPadCentered and !leftColorChosen){
        if (ACon_Z){
          leftColorChosen = true;
        }
        if(ACon_X > 100 or ACon_X < -100){ 
          selectionflag++;
        }
        if (ACon_X > 100 and colorSelectionLeft+1 <= colormax and selectionflag > 15){
          colorSelectionLeft++;
          selectionflag = 0;
        }
        if (ACon_X < -100 and colorSelectionLeft-1 >= colormin and selectionflag > 15){
          colorSelectionLeft--;
          selectionflag = 0;
        }

        //Update colors on the paddle LEDs
        if (colorSelectionLeft == 0){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Blue;    
          }
          for(int i = 0; i < 47; i++) {
            borderLED[i] = CRGB::Blue;    
          }
          FastLED.show(); 
        }
        if (colorSelectionLeft == 1){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Red;    
          }
          for(int i = 0; i < 47; i++) {
            borderLED[i] = CRGB::Red;    
          }
          FastLED.show(); 
        }
        if (colorSelectionLeft == 2){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Green;    
          }
          for(int i = 0; i < 47; i++) {
            borderLED[i] = CRGB::Green;    
          }
          FastLED.show(); 
        }
        if (colorSelectionLeft == 3){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Magenta;    
          }
          for(int i = 0; i < 47; i++) {
            borderLED[i] = CRGB::Magenta;    
          }
          FastLED.show(); 
        }
        if (colorSelectionLeft == 4){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Cyan;    
          }
          for(int i = 0; i < 47; i++) {
            borderLED[i] = CRGB::Cyan;    
          }
          FastLED.show(); 
        }
        if (colorSelectionLeft == 5){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Gold;    
          }
          for(int i = 0; i < 47; i++) {
            borderLED[i] = CRGB::Gold;    
          }
          FastLED.show(); 
        }
      }

      //Once left color is seected, move right paddle up
      if (leftPadCentered and leftColorChosen and !rightPadCentered){
        StepControl paddleSteppers;
        paddleSteppers.move(rightPad);
        rightPadCentered = true;
        rpadtarget = int(paddlemax/2);
        selectionflag = 0;
      }

      //Use right nunchuck to scroll thru color choices
      if (leftPadCentered and leftColorChosen and rightPadCentered and !rightColorChosen){
        if (BCon_Z){
          rightColorChosen = true;
        }
        if(BCon_X > 100 or BCon_X < -100){ 
          selectionflag++;
        }
        if (BCon_X > 100 and colorSelectionRight+1 <= colormax and selectionflag > 15){
          colorSelectionRight++;
          selectionflag = 0;
        }
        if (BCon_X < -100 and colorSelectionRight-1 >= colormin and selectionflag > 15){
          colorSelectionRight--;
          selectionflag = 0;
        }

        //Update colors on the paddle LEDs
        if (colorSelectionRight == 0){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Blue;    
          }
          for(int i = 46; i < NUM_LEDS_BORDER; i++) {
            borderLED[i] = CRGB::Blue;    
          }
          FastLED.show(); 
        }
        if (colorSelectionRight == 1){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Red;    
          }
          for(int i = 46; i < NUM_LEDS_BORDER; i++) {
            borderLED[i] = CRGB::Red;    
          }
          FastLED.show(); 
        }
        if (colorSelectionRight == 2){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Green;    
          }
          for(int i = 46; i < NUM_LEDS_BORDER; i++) {
            borderLED[i] = CRGB::Green;    
          }
          FastLED.show(); 
        }
        if (colorSelectionRight == 3){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Magenta;    
          }
          for(int i = 46; i < NUM_LEDS_BORDER; i++) {
            borderLED[i] = CRGB::Magenta;    
          }
          FastLED.show(); 
        }
        if (colorSelectionRight == 4){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Cyan;    
          }
          for(int i = 46; i < NUM_LEDS_BORDER; i++) {
            borderLED[i] = CRGB::Cyan;    
          }
          FastLED.show(); 
        }
        if (colorSelectionRight == 5){
          for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Gold;    
          }
          for(int i = 46; i < NUM_LEDS_BORDER; i++) {
            borderLED[i] = CRGB::Gold;    
          }
          FastLED.show(); 
        }
      }

      //Move on once both sides have chosen their color
      if (leftPadCentered and leftColorChosen and rightPadCentered and rightColorChosen){
        machineState = countdown;
      }

      delay(10);
      break;
    }

    case gameplay:{ //PONG game mechanics

      /* Set speed and acceleration to highest the 
       * mechanical system can handle without losing steps
       */
      leftPad.setAcceleration(5000); // steps/s^2 
      rightPad.setAcceleration(5000); // steps/s^2*/

      LBall.setMaxSpeed(5000);       // steps/s
      LBall.setAcceleration(10000); // steps/s^2

      RBall.setMaxSpeed(5000);       // steps/s
      RBall.setAcceleration(10000); // steps/s^2

      //Keep track of motor positions
      leftPadPos = leftPad.getPosition();
      rightPadPos = rightPad.getPosition();

      //Move left pad according to player input
      if (ACon_Y > 100 and leftPadPos+100 <= paddlemax){
          leftPad.setMaxSpeed(2000);      
          if (lpadmovecomplete){
            leftPaddleStepper.rotateAsync(leftPad);
            lpadmovecomplete = false;
          }
      }
      if (ACon_Y < -100 and leftPadPos-100 >= 0){
          leftPad.setMaxSpeed(-2000);      
          if (lpadmovecomplete){
            leftPaddleStepper.rotateAsync(leftPad);
            lpadmovecomplete = false;
          }
      }
      if (ACon_Y > -100 and ACon_Y < 100){
        leftPaddleStepper.stopAsync();
        lpadmovecomplete = true;
      }

      //Move right pad according to player input
      if (BCon_Y > 100 and rightPadPos+100 <= paddlemax){
        rightPad.setMaxSpeed(2000);      
        if (rpadmovecomplete){
          rightPaddleStepper.rotateAsync(rightPad);
          rpadmovecomplete = false;
        }
      }
      if (BCon_Y < -100 and rightPadPos-100 >= 0){
        rightPad.setMaxSpeed(-2000);      
        if (rpadmovecomplete){
          rightPaddleStepper.rotateAsync(rightPad);
          rpadmovecomplete = false;
        }
      }
      if (BCon_Y > -100 and BCon_Y < 100){
        rightPaddleStepper.stopAsync();
        rpadmovecomplete = true;
      }

//      Serial.print("left: ");
//      Serial.print(leftPadPos);
//      Serial.print("     right: ");
//      Serial.println(rightPadPos);

      //At the start of the round, pick a random location 
      //on the right side and shoot the ball over ther
      int deltax = 0;
      int deltay = 0;
      if (!gamebegun){ 
        deltax = int(ball_horizontal_max/2);
        deltay = int(random(int(-ball_vertical_max/2),int(ball_vertical_max/2)));
               
        targetposabsLeft = LBall.getPosition()+calculateStepsLeft(deltax,deltay);
        targetposabsRight = RBall.getPosition()+calculateStepsRight(deltax,deltay);
      
        currentBallPosX = currentBallPosX + deltax;
        currentBallPosY = currentBallPosY + deltay;
        multiplier = -1;
        gamebegun = true;
       }
       
      LBall.setTargetAbs(targetposabsLeft);
      RBall.setTargetAbs(targetposabsRight);
      if (ballmovecomplete){
        ballSteppers.moveAsync(LBall,RBall);
        ballmovecomplete = false;
      }

      //When ball gets to its target on the left or right wall, its motion is complete
      if ((LBall.getPosition()+1 == targetposabsLeft or LBall.getPosition()-1 == targetposabsLeft or LBall.getPosition() == targetposabsLeft) and (RBall.getPosition()+1 == targetposabsRight or RBall.getPosition()-1 == targetposabsRight or RBall.getPosition() == targetposabsRight)){
        ballmovecomplete = true;
        deltax = 0;
        deltay = 0;

        /* 'multiplier' tracks whether the ball going to the right player (-1)
         *  or the left player(1) and who is responsible for hitting it
         *  
         * The ball moves only between the two player walls, but its vertical 
         * position can be influenced by the player. 
         * 
         * If the ball makes contact witrh the top half of a paddle,
         * its next location will be randomly chosen to be somewhere on 
         * the top half of the opposing player's wall. If the ball makes 
         * contact with the bottom half of the paddle,its next location 
         * will be randomly chosen to be somewhere on the bottom half 
         * of the opposing player's wall.
         * 
         * This lets the player have some control of where to send the ball.
         */
         
        if (multiplier<0){
          //If the paddle and the ball happen to be in the same place, the player has 'hit' the ball
          if ((rightPadPos+halfpaddlesteps) >= currentBallPosY and (rightPadPos) <= currentBallPosY){
            deltax = multiplier*ball_horizontal_max;
            int ball_pos_rel = currentBallPosY-ball_vertical_half;
            deltay = int(random(0,(ball_vertical_half-ball_pos_rel)));
               
            targetposabsLeft = LBall.getPosition()+calculateStepsLeft(deltax,deltay);
            targetposabsRight = RBall.getPosition()+calculateStepsRight(deltax,deltay);

            currentBallPosX = currentBallPosX + deltax;
            currentBallPosY = currentBallPosY + deltay;

            //Serial.println("HIT");
            multiplier = multiplier*-1;
          }
          else if((rightPadPos) >= currentBallPosY and (rightPadPos-halfpaddlesteps) <= currentBallPosY){
            deltax = multiplier*ball_horizontal_max;
            int ball_pos_rel = currentBallPosY-ball_vertical_half;
            deltay = int(random((-ball_vertical_half-ball_pos_rel),0));
               
            targetposabsLeft = LBall.getPosition()+calculateStepsLeft(deltax,deltay);
            targetposabsRight = RBall.getPosition()+calculateStepsRight(deltax,deltay);

            currentBallPosX = currentBallPosX + deltax;
            currentBallPosY = currentBallPosY + deltay;

            //Serial.println("HIT");
            multiplier = multiplier*-1;
          }
          //Otherwise, the player has missed the ball
          else{
            rightPaddleStepper.stopAsync();
            leftPaddleStepper.stopAsync();
            lpadmovecomplete = true;
            rpadmovecomplete = true;
            leftscore++;
            scoreLeft(colorSelectionLeft,leftscore);
            if (leftscore == 10){
              machineState = leftwin;
              break;
            }
            else{
              delay(800);
              machineState = restore;
            }
          }
        }
        //If the paddle and the ball happen to be in the same place, the player has 'hit' the ball
        else if (multiplier>0){
          if ((leftPadPos+halfpaddlesteps) >= currentBallPosY and (leftPadPos) <= currentBallPosY ){
            deltax = multiplier*ball_horizontal_max;
            int ball_pos_rel = currentBallPosY-ball_vertical_half;
            deltay = int(random(0,(ball_vertical_half-ball_pos_rel))); 
               
            targetposabsLeft = LBall.getPosition()+calculateStepsLeft(deltax,deltay);
            targetposabsRight = RBall.getPosition()+calculateStepsRight(deltax,deltay);

            currentBallPosX = currentBallPosX + deltax;
            currentBallPosY = currentBallPosY + deltay;

            //Serial.println("HIT");
            multiplier = multiplier*-1;
          }
          else if((leftPadPos) >= currentBallPosY and (leftPadPos-halfpaddlesteps) <= currentBallPosY ){
            deltax = multiplier*ball_horizontal_max;
            int ball_pos_rel = currentBallPosY-ball_vertical_half;
            deltay = int(random((-ball_vertical_half-ball_pos_rel),0)); 
               
            targetposabsLeft = LBall.getPosition()+calculateStepsLeft(deltax,deltay);
            targetposabsRight = RBall.getPosition()+calculateStepsRight(deltax,deltay);

            currentBallPosX = currentBallPosX + deltax;
            currentBallPosY = currentBallPosY + deltay;

            //Serial.println("HIT");
            multiplier = multiplier*-1;
          }
          //Otherwise, the player has missed the ball
          else{
            rightPaddleStepper.stopAsync();
            leftPaddleStepper.stopAsync();
            lpadmovecomplete = true;
            rpadmovecomplete = true;
            rightscore++;
            scoreRight(colorSelectionRight,rightscore);
            if (rightscore == 10){
              machineState = rightwin;
              break;
            }
            else{
              delay(800);
              machineState = restore;
            }
          }
        }
      }

      //A small delay prevents any weird timing glitches
      delay(15);
      break;
    }
    
    case countdown: { //Show countdown sequence on ball LEDs
      //Serial.print("countdown");

      //Display "3" on ball
      for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Green;    
        }
      for(int i = 5; i < 7; i++) {
          ballLED[i] = CRGB::White;    
        }
      for(int i = 13; i < 15; i++) {
          ballLED[i] = CRGB::White;    
        }
      for(int i = 21; i < 23; i++) {
          ballLED[i] = CRGB::White;    
        }
      FastLED.show();
      delay (800);
      
      //Display "2" on ball 
      for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Green;    
        }
      for(int i = 9; i < 11; i++) {
          ballLED[i] = CRGB::White;    
        }
      for(int i = 17; i < 19; i++) {
          ballLED[i] = CRGB::White;    
        }
      FastLED.show();
      delay (800);

      //Display "1" on ball
      for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Green;    
        }
      for(int i = 13; i < 15; i++) {
          ballLED[i] = CRGB::White;    
        }
      FastLED.show();
      delay (800);

      //Set ball color to white
      for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::White;    
        }

      //Clear scoreboard (if this is first round)  
      if (clearscoreboard){
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Black;    
        }
        clearscoreboard = false;
      }
      FastLED.show();
      
      machineState = gameplay;
      break;
    }
    
    case restore: { //Move ball to center befor every round
      
      //Only turn paddles red if colors haven't been chosen yet
      if (!leftColorChosen and !rightColorChosen){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
          leftPadLED[i] = CRGB::Red;    
        }
        
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
          rightPadLED[i] = CRGB::Red;    
        }
      }

      //Make ball red as it moves to start position
      for(int i = 0; i < NUM_LEDS_BALL; i++) {
        ballLED[i] = CRGB::Red;    
      }
      FastLED.show(); 

      //Send the ball to the center
      StepControl ballSteppers; 
      LBall.setMaxSpeed(5000);       // steps/s
      LBall.setAcceleration(10000); // steps/s^2

      RBall.setMaxSpeed(5000);       // steps/s
      RBall.setAcceleration(10000); // steps/s^2
      
      int deltay =  ball_vertical_max/2 - currentBallPosY;
      int deltax =  ball_horizontal_max/2 - currentBallPosX;

      LBall.setTargetRel(calculateStepsLeft(deltax,deltay));
      RBall.setTargetRel(calculateStepsRight(deltax,deltay));
      
      ballSteppers.moveAsync(LBall,RBall);
      delay (2000);

      //If colors haven't been chosen yet, make paddles white
      if (!leftColorChosen and !rightColorChosen){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::White;    
          }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
          rightPadLED[i] = CRGB::White;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
        borderLED[i] = CRGB::Green;    
        }
      }

      //Turn ball LEDs freen to indicate ready state
      for(int i = 0; i < NUM_LEDS_BALL; i++) {
        ballLED[i] = CRGB::Green;    
      }
      FastLED.show(); 

      //Reset ball position
      currentBallPosX = int(ball_horizontal_max/2);
      currentBallPosY = int(ball_vertical_max/2);
      RBall.setPosition(500000);  
      LBall.setPosition(500000); 
      gamebegun = false;
      lpadmovecomplete = true;
      rpadmovecomplete = true;
      
      machineState = colorSelect;
      break;
    }

    case leftwin:{ //Win sequence for left player
      //Move ball to the middle
      StepControl allSteppers; 
      LBall.setMaxSpeed(5000);      
      LBall.setAcceleration(8000); 
      RBall.setMaxSpeed(5000);     
      RBall.setAcceleration(8000); 
      
      int deltay =  ball_vertical_max/2 - currentBallPosY;
      int deltax =  ball_horizontal_max/2 - currentBallPosX;
      
      LBall.setTargetRel(calculateStepsLeft(deltax,deltay));
      RBall.setTargetRel(calculateStepsRight(deltax,deltay));
      
      ballSteppers.moveAsync(LBall,RBall);
      delay (2000);
      
      currentBallPosX = int(ball_horizontal_max/2);
      currentBallPosY = int(ball_vertical_max/2);

      //Lower right paddle and lift left paddle to center
      leftPad.setMaxSpeed(5000);       
      leftPad.setAcceleration(8000); 
      rightPad.setMaxSpeed(5000);     
      rightPad.setAcceleration(8000); 
      
      leftPad.setTargetAbs(0);
      rightPad.setTargetAbs(0);
      allSteppers.moveAsync(leftPad,rightPad);
      delay(2000);
     
      leftPad.setTargetAbs(int(paddlemax/2));
      allSteppers.moveAsync(leftPad);
      delay(1000);

      //Set all LEDs to the same color as the winning side
      if (colorSelectionLeft == 0){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
          leftPadLED[i] = CRGB::Blue;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
          rightPadLED[i] = CRGB::Blue;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Blue;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Blue;    
        }
        FastLED.show(); 
      }
      if (colorSelectionLeft == 1){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Red;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Red;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Red;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Red;    
        }
        FastLED.show(); 
      }
      if (colorSelectionLeft == 2){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Green;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Green;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Green;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Green;    
        }
        FastLED.show(); 
      }
      if (colorSelectionLeft == 3){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Magenta;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Magenta;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Magenta;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Magenta;    
        }
        FastLED.show(); 
      }
      if (colorSelectionLeft == 4){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Cyan;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Cyan;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Cyan;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Cyan;    
        }
        FastLED.show(); 
      }
      if (colorSelectionLeft == 5){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Gold;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Gold;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Gold;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Gold;    
        }
        FastLED.show(); 
      }
      
      //Blink it a few times to loser feel bad
      delay (800);  
      FastLED.setBrightness(0);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(0);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(0);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();

      //Bring the left paddle back down
      leftPad.setTargetAbs(0);
      allSteppers.moveAsync(leftPad);
      delay(1000);

      //Reset everything
      leftPadCentered = false;
      leftColorChosen = false;
      rightPadCentered = false;
      rightColorChosen = false;
      colorSelectionLeft = 0;
      colorSelectionRight = 0;
      gamebegun = false;
      clearscoreboard = true;
      leftscore = 0;
      rightscore = 0;

      //Drop the ball and re-enter homing sequence
      deltay =  -ball_vertical_max/2+100;
      deltax =  0;

      LBall.setTargetRel(calculateStepsLeft(deltax,deltay));
      RBall.setTargetRel(calculateStepsRight(deltax,deltay));
      
      allSteppers.moveAsync(LBall,RBall);
      delay (2000);
      RBall.setPosition(0);  
      LBall.setPosition(0); 
      machineState = homing;
      break;
    }

    case rightwin:{ //Win sequence for right player
      //Move ball to the middle
      StepControl allSteppers; 
      LBall.setMaxSpeed(5000);       
      LBall.setAcceleration(8000); 
      RBall.setMaxSpeed(5000);       
      RBall.setAcceleration(8000); 
      
      int deltay =  ball_vertical_max/2 - currentBallPosY;
      int deltax =  ball_horizontal_max/2 - currentBallPosX;
      
      LBall.setTargetRel(calculateStepsLeft(deltax,deltay));
      RBall.setTargetRel(calculateStepsRight(deltax,deltay));
      
      ballSteppers.moveAsync(LBall,RBall);
      delay (2000);

      currentBallPosX = int(ball_horizontal_max/2);
      currentBallPosY = int(ball_vertical_max/2);
      
      //Lower left paddle and lift right paddle to center      
      leftPad.setMaxSpeed(5000);       
      leftPad.setAcceleration(8000); 
      rightPad.setMaxSpeed(5000);       
      rightPad.setAcceleration(8000); 
       
      leftPad.setTargetAbs(0);
      rightPad.setTargetAbs(0);

      allSteppers.moveAsync(leftPad,rightPad);
      delay (2000);
       
      rightPad.setTargetAbs(int(paddlemax/2));
      allSteppers.moveAsync(rightPad);
      delay (1000);

      //Set all LEDs to the same color as the winning side
      if (colorSelectionRight == 0){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
          leftPadLED[i] = CRGB::Blue;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
          rightPadLED[i] = CRGB::Blue;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Blue;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Blue;    
        }
        FastLED.show(); 
      }
      if (colorSelectionRight == 1){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Red;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Red;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Red;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Red;    
        }
        FastLED.show(); 
      }
      if (colorSelectionRight == 2){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Green;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Green;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Green;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Green;    
        }
        FastLED.show(); 
      }
      if (colorSelectionRight == 3){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Magenta;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Magenta;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Magenta;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Magenta;    
        }
        FastLED.show(); 
      }
      if (colorSelectionRight == 4){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Cyan;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Cyan;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Cyan;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Cyan;    
        }
        FastLED.show(); 
      }
      if (colorSelectionRight == 5){
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            leftPadLED[i] = CRGB::Gold;    
        }
        for(int i = 0; i < NUM_LEDS_PAD; i++) {
            rightPadLED[i] = CRGB::Gold;    
        }
        for(int i = 0; i < NUM_LEDS_BORDER; i++) {
          borderLED[i] = CRGB::Gold;    
        }
        for(int i = 0; i < NUM_LEDS_BALL; i++) {
          ballLED[i] = CRGB::Gold;    
        }
        FastLED.show(); 
      }

      //Blink it a few times to really make the loser feel bad
      delay (800);  
      FastLED.setBrightness(0);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(0);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(0);
      FastLED.show();
      delay (800);  
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();

      //Bring the right paddle back down
      rightPad.setTargetAbs(0);
      allSteppers.moveAsync(rightPad);
      delay(1000);

      //Reset everything
      leftPadCentered = false;
      leftColorChosen = false;
      rightPadCentered = false;
      rightColorChosen = false;
      colorSelectionLeft = 0;
      colorSelectionRight = 0;
      gamebegun = false;
      clearscoreboard = true;
      leftscore = 0;
      rightscore = 0;

      //Drop the ball and re-enter homing sequence
      deltay =  -ball_vertical_max/2+100;
      deltax =  0;
      
      LBall.setTargetRel(calculateStepsLeft(deltax,deltay));
      RBall.setTargetRel(calculateStepsRight(deltax,deltay));
      
      allSteppers.moveAsync(LBall,RBall);
      delay (2000);
      
      RBall.setPosition(0);  
      LBall.setPosition(0); 
      machineState = homing;
      break;
    }
  }
}
