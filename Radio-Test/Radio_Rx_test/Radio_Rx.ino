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
  int inByte;
  char command = 0;
  char joystick = 0;
  char axis = 0;
  int phase = 1;
  int inc = 0;
  char inputStr[10] = "0.00";
  float input = 0;
  int negInput = 0;
  
  
  while(phase != 5){


    // Read a single byte from the buffer
    if(radio.available()){
      while(radio.available()){
        radio.read(&inByte, 1);

        flash();
        Serial.print("recieved byte: ");
        Serial.println(char(inByte));

        // Determine data processing phase
        
        if(phase == 1){                       // Process Command Char
          command == char(inByte);
          
          if(command != '<'){
            phase = 5;
          }
          else {
            phase = 2;
            negInput = 0;
          }
        }
        else if(phase == 2){                  // Process Joystick Char
          joystick = char(inByte);
          
          if(joystick != '1' && joystick != '2'){
            phase = 1; 
          }
          else {
            phase = 3;
          }
        }
        else if(phase == 3){                  // Process Axis Char
          axis = char(inByte);

          if(axis != '1' && axis != '2'){
            phase = 1; 
          }
          else {
            phase = 4;
          }
        }
        else if(phase == 4){                  // Process Input Float
          if(char(inByte) == '>'){
            input = String(inputStr).toFloat();
            if(negInput == 1){
              input = input * -1;
            }
            phase = 5;
          }
          if(char(inByte) == '-'){
            negInput = 1;
          }
          else {
            inputStr[inc] = char(inByte);
            inc++;
          }
        }
      }
    }
  }
  if(command == '<') {
    Serial.println("----JOYSTICK INPUT----");
    Serial.print("joystick: ");
    Serial.println(joystick);
    Serial.print("axis: ");
    Serial.println(axis);
    Serial.print("Input: ");
    Serial.println(inputStr);
  }
  else {
    Serial.println("----BUTTON INPUT----");
    Serial.print("command: ");
    Serial.println(command);
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
