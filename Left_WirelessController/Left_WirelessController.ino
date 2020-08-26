/*
 * Wireless Controller A
 * Written by: Prajwal Tumkur Mahesh
 *
 * Upload this code to the controller for the left paddle
 */
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include "Nunchuk.h"

//nRF24L01+ Configuration
RF24 radio(11, 10); //(CE,CSN)
RF24Network network(radio);      
const uint16_t this_node = 01;     // Address of this node
const uint16_t master00 = 00;      // Address of the master node in Octal format
const unsigned long interval = 10; // How often to send data to master unit (ms)
unsigned long last_sent;

void setup() {
  //Initialize transmit radio
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);

  //Initialize I2C to talk to nunchuck board
  Wire.begin();
  Wire.setClock(400000);
  nunchuk_init();
}

void loop() {
  network.update();

  //Get and send data periodically
  if (millis() - last_sent >= interval) {   
    last_sent = millis();

    //Collect inputs from nunchuck
    int nunchuck_xval = 0;
    int nunchuck_yval = 0;
    int nunchuck_zval = 0;
    if (nunchuk_read()) {
      nunchuck_xval = nunchuk_joystickX();
      nunchuck_yval = nunchuk_joystickY();
      nunchuck_zval = nunchuk_buttonZ();
    }

    //Format the read values and stick them into a nice array
    int nunchuck_values[3];
    nunchuck_values[0] = nunchuck_xval;
    nunchuck_values[1] = nunchuck_yval+1000;
    nunchuck_values[2] = nunchuck_zval+2000;

    // Send the data to the master controller
    RF24NetworkHeader header(master00);   
    for (int i = 0; i < 3; i++){
      bool ok = network.write(header, &nunchuck_values[i], sizeof(nunchuck_values[i])); // Send the data
    }
    
  }
}
