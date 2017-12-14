
#include <SPI.h>
#include "RF24.h"
/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
byte addresses[][6] = {"1Node","2Node"};

struct dataStruct{
  int command;
  int joystick;
  int axis;
//  float input;
//  String inputStr;
  int endChar;
}myData;

void setup() {
  Serial.begin(250000);
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS); 
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.stopListening();

}

void loop() {
//  int joystick;
//  int axis;
//  float input;
//  String inputStr;
//  int command;
//  int endChar;

  while(1){
//    radio.stopListening();

    // Clear previous input values
    myData.command = 0;
    myData.joystick = 0;
    myData.axis = 0;
//    myData.input = 0;
//    myData.inputStr = '0';
    myData.endChar = 0;
    
    if(Serial.available()){
    
      myData.command = Serial.read();
        
      if(myData.command == '<'){
        myData.joystick = Serial.parseInt();
        myData.axis = Serial.parseInt();
        myData.input = Serial.parseFloat();
        myData.endChar = Serial.read();

//        inputStr = String(input);

        radio.write(&myData, sizeof(myData));
        
//        radio.write(&command,1);
//        radio.write(&joystick,1);
//        radio.write(&axis,1);
        //radio.write(&input,sizeof(input));
//        radio.write(&endChar,1);
//        radio.startListening();
      }
      else {
        radio.write(&myData,sizeof(myData));
      }
    }
  }
}
