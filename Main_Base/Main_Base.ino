/*
 * Mechanical PONG 
 * Written by: Prajwal Tumkur Mahesh
 *  
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <FastLED.h>
#include "TeensyStep.h"

//Setup for PONG state machine
enum MachineStates {homing, colorSelect, gameplay, countdown, restore, leftwin, rightwin};
MachineStates machineState = homing;

//Configure stepper driver pins
Stepper  leftPad(2, 3); //(STEP,DIR)
Stepper  rightPad(4,5); 
Stepper  LBall(6,7); 
Stepper  RBall(8,9);  

//Create stepper controllers
StepControl ballSteppers;
RotateControl leftPaddleStepper;
RotateControl rightPaddleStepper; 

//Pin connections
#define leftPadEn 27
#define rightPadEn 26
#define RBallEn 25
#define LBallEn 24

#define ballZminStop 18
#define ballXmaxStop 19

#define leftPadLEDPin 23
#define rightPadLEDPin 22
#define ballLEDPin 21
#define borderLEDPin 20

//LED configuration
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define NUM_LEDS_PAD 27
#define NUM_LEDS_BALL 28
#define NUM_LEDS_BORDER 93
#define BRIGHTNESS  200

//Give the colors easier names
#define blue CRGB:Blue
#define red CRGB:Red
#define green CRGB:Green
#define magenta CRGB:Magenta
#define cyan CRGB:Cyan
#define gold CRGB:Gold

//Create separate LED controller for each element
CRGB leftPadLED[NUM_LEDS_PAD];
CRGB rightPadLED[NUM_LEDS_PAD];
CRGB ballLED[NUM_LEDS_BALL];
CRGB borderLED[NUM_LEDS_BORDER];

//Motion configuration variables
const float paddle_steps_per_mm = 6.67;
const int paddle_range_in_mm = 840;
const int paddlelength_in_mm = 150;
const int paddlemax = int(paddle_steps_per_mm*paddle_range_in_mm);
const int halfpaddlesteps = int(paddle_steps_per_mm*paddlelength_in_mm/2);
int leftPadPos = 0;
int rightPadPos = 0;

const float ball_step_per_mm = 6.67;
const int ball_range_horizontal_in_mm = 1260;
const int ball_range_vertical_in_mm = 800;
const int ball_horizontal_max = int(ball_step_per_mm*ball_range_horizontal_in_mm);
const int ball_vertical_max = int(ball_step_per_mm*ball_range_vertical_in_mm);
const int ball_vertical_half = int(ball_vertical_max/2);
int ballxPos = 0;
int ballZPos = 0;

//nRF24L01+ Configuration
RF24 radio(10,28); //(CE,CSN)
RF24Network network(radio);      
const uint16_t this_node = 00;   // Address of this node(master)
const uint16_t node01 = 01;      // Address of left controller node
const uint16_t node02 = 02;      // Address of right controller node

//Variables to store received controller data
int BCon_Z = 0;
int BCon_Y = 0;
int BCon_X = 0;

int ACon_Z = 0;
int ACon_Y = 0;
int ACon_X = 0;

//Gameplay variables
bool leftPadCentered = false;
bool leftColorChosen = false;
bool rightPadCentered = false;
bool rightColorChosen = false;
int colorSelectionLeft = 0;
int colorSelectionRight = 0;
const int colormax = 500;
const int colormin = 0;
long lasttime = 0;
int delaytime = 5;
int selectionflag = 0;

bool gamebegun = false;
int currentBallPosX = 0;
int currentBallPosY = 0;
int targetposabsLeft = 0;
int targetposabsRight = 0;
int lpadtarget = 0;
int rpadtarget = 0;
bool ballmovecomplete = true;
bool lpadmovecomplete = true;
bool rpadmovecomplete = true;
int multiplier = -1;
int leftscore = 0;
int rightscore = 0;
bool clearscoreboard = true;

void setup() {
  leftPad.setMaxSpeed(5000);       // steps/s
  leftPad.setAcceleration(8000); // steps/s^2 
  leftPad.setPosition(0);    
  
  rightPad.setMaxSpeed(5000);       // steps/s
  rightPad.setAcceleration(8000); // steps/s^2
  rightPad.setPosition(0); 
     
  LBall.setMaxSpeed(10000);       // steps/s
  LBall.setAcceleration(10000); // steps/s^2

  RBall.setMaxSpeed(10000);       // steps/s
  RBall.setAcceleration(10000); // steps/s^2

  //Initialize stepper driver enable pins
  pinMode(leftPadEn, OUTPUT);
  pinMode(rightPadEn, OUTPUT);
  pinMode(RBallEn, OUTPUT);
  pinMode(LBallEn, OUTPUT);

  digitalWrite(leftPadEn, LOW);
  digitalWrite(rightPadEn, LOW);
  digitalWrite(RBallEn, LOW);
  digitalWrite(LBallEn, LOW);

  //Initialize endstop pins
  pinMode (ballZminStop, INPUT_PULLUP);
  pinMode (ballXmaxStop, INPUT_PULLUP);

  //Set up LEDs
  FastLED.addLeds<CHIPSET, leftPadLEDPin, COLOR_ORDER>(leftPadLED, NUM_LEDS_PAD).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, rightPadLEDPin, COLOR_ORDER>(rightPadLED, NUM_LEDS_PAD).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, ballLEDPin, COLOR_ORDER>(ballLED, NUM_LEDS_BALL).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, borderLEDPin, COLOR_ORDER>(borderLED, NUM_LEDS_BORDER).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  //Make them all Red
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

  //Initialize receive radio
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  
  //For debugging
  Serial.begin(9600);
}

void loop() {
  network.update();

  //Interpret data when it's received
  while ( network.available() ) {     
    RF24NetworkHeader header;
    int incomingData;
    // Read the incoming data
    network.read(header, &incomingData, sizeof(incomingData)); 

    //If received data is from the right controller 
    if (header.from_node == 2) {   
      if (incomingData >= 2000 and incomingData <= 2001){
        BCon_Z = incomingData - 2000;
      }
      if (incomingData >= 500 and incomingData <= 1500 ){
        BCon_Y = incomingData - 1000;
      }
      if (incomingData <= 500 ){
        BCon_X = incomingData-1;
      }
      else if (incomingData >= 65410 and incomingData <= 65535){
        BCon_X = incomingData-65535; 
      }
    }
    
    //If received data is from the left controller 
    if (header.from_node == 1) {  
      if (incomingData >= 2000 and incomingData <= 2001){
        ACon_Z = incomingData - 2000;
      }
      if (incomingData >= 500 and incomingData <= 1500 ){
        ACon_Y = incomingData - 1000;
      }
      if (incomingData <= 500 ){
        ACon_X = incomingData-1;
      }
      else if (incomingData >= 65410 and incomingData <= 65535){
        ACon_X = incomingData-65535; 
      }
    }
  }

  //Run the game
  PONG();
}
