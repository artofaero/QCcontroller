////// Begin Definitions \\\\\\

// Libraries
#include <SPI.h>
#include "RF24.h"

// Motor Pins
int Y = 5;
int B = 4;
int G = 3;
int R = 2;

// Radio Setup
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(53,48);
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
  // set pin modes
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Y, OUTPUT);

  // init
  Serial.begin(115200);
  radioSetup();

  flash();
}

void loop() {
  int command;
  int joystick;

  while(1){

    // Clear previous input values
    myData.command = 0;
    myData.joystick = 0;
    myData.axis = 0;
//    myData.input = 0;
//    myData.inputStr = '0';
    myData.endChar = 0;
    
    if(radio.available()){

      while(radio.available()){
        radio.read(&myData, sizeof(myData));
          
//        radio.read(&command,1);
        flash();
        Serial.print("command: ");
        Serial.println(char(myData.command));
        Serial.print("joystick: ");
        Serial.println(char(myData.joystick));
        Serial.print("axis: ");
        Serial.println(char(myData.axis));
        Serial.print("endChar: ");
        Serial.println(char(myData.endChar));
                
//        if(command == 60){
//          flash();
//          Serial.print("command: ");
//          Serial.println(command);
//          radio.read(&joystick,1);
//          Serial.print('joystick: ');
//          Serial.println(joystick);
//        }
      }
    }
  }
}

void radioSetup(){
  pinMode(53,OUTPUT);
  radio.begin();
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
  radio.startListening();
}

void flash(){
  digitalWrite(R,HIGH);
  digitalWrite(G,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(Y,HIGH);
  
  delay(150);

  digitalWrite(R,LOW);
  digitalWrite(G,LOW);
  digitalWrite(B,LOW);
  digitalWrite(Y,LOW);
}
