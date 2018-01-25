// Libraries
#include <SPI.h>
#include "RF24.h"

// Motor Pins
int Y = 5;
int B = 4;
int G = 3;
int R = 2;

float e = 0.5; // Throttle Fraction
float f = 0.2; // Yaw Fraction
float g = 0.15; // Roll Fraction
float h = 0.15; // Pitch Fraction

/******************** Radio Setup *************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(53,48);
byte addresses[][6] = {"1Node","2Node"};
/*********************************************************/

/****************** Data Handling Globals ****************/
const byte numChars = 32;
char buff[numChars];
boolean newData = false;
/*********************************************************/

/******** Quad Copter Flight Controller Simulation *******/
  // Constant Roll/Pitch Control Scheme
  //
  // Quad-Copter Motor Layout:
  //
  //         Front
  //           ^
  //     R     |      B
  //     cw          ccw
  //      (0)     (1)
  //         %   % 
  //          % %
  //           @
  //          % %
  //         %   %
  //      (2)     (1)
  //     ccw         cw
  //     Y            G

// Inputs:
  // t = throttle input. Number between 0 and 1.
  // y = Yaw input. Number between 0 and 1.
  // r = Roll input. Number between 0 and 1.
  // p = Pitch input. Number between 0 and 1.
  //
  // e = thrust fraction. Number between 0 and 1 following these rules:
  // f = Yaw Fraction. Number between 0 and 1 following these rules:
  // g = Roll Fraction. Number between 0 and 1 following these rules:
  // h = Pitch Fraction. Number between 0 and 1 following these rules:
  //       e + f + g + h <= 1
  //       f + g + h <= e
/************************************************************************/

struct cmdline {
  char command;
  char joystick;
  char axis;
  String inputStr;
  float input;
};

void setup() {
  // set pin modes
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Y, OUTPUT);

  // init
  Serial.begin(115200);

  flash();
}

void loop() {
  struct cmdline theCMD;
 
  theCMD = readRadioBuff();
//  printInput(theCMD.command, theCMD.joystick, theCMD.axis, theCMD.inputStr, theCMD.input);
//  if(theCMD.command == '!') {
//    if(theCMD.joystick == '1'){
//      if(theCMD.axis == '1'){
//        theCMD.input *= -1;
//        analogWrite(R,floatTOint(theCMD.input));
//        analogWrite(Y,floatTOint(theCMD.input));
//        analogWrite(G,floatTOint(theCMD.input));
//        analogWrite(B,floatTOint(theCMD.input));
//      }
//      else if(theCMD.axis == '2'){
//        if(theCMD.input > 0){
//          digitalWrite(Y,LOW);
//          digitalWrite(R,HIGH);
//          digitalWrite(G,HIGH);
//          digitalWrite(B,LOW);
//        }
//        else if(theCMD.input < 0){
//          digitalWrite(Y,HIGH);
//          digitalWrite(R,LOW);
//          digitalWrite(G,LOW);
//          digitalWrite(B,HIGH);
//        }
//      }
//    }
//    else if(theCMD.joystick == '2'){
//      if(theCMD.axis == '1'){
//        if(theCMD.input < 0){
//          digitalWrite(B,HIGH);
//          digitalWrite(R,HIGH);
//          digitalWrite(Y,LOW);
//          digitalWrite(G,LOW);
//        }
//        else if(theCMD.input > 0){
//          digitalWrite(B,LOW);
//          digitalWrite(R,LOW);
//          digitalWrite(Y,HIGH);
//          digitalWrite(G,HIGH);
//        }
//      }
//      else if(theCMD.axis == '2'){
//        if(theCMD.input < 0){
//          digitalWrite(B,LOW);
//          digitalWrite(R,HIGH);
//          digitalWrite(Y,HIGH);
//          digitalWrite(G,LOW);
//        }
//        else if(theCMD.input > 0){
//          digitalWrite(B,HIGH);
//          digitalWrite(R,LOW);
//          digitalWrite(Y,LOW);
//          digitalWrite(G,HIGH);
//        }
//      } 
//    }
//  }
}

// Functions
void printInput(char command, char joystick, char axis, String inputStr, float input){
  if(command == '!') {
    Serial.println("----JOYSTICK INPUT----");
    Serial.print("joystick: ");
    Serial.println(joystick);
    Serial.print("axis: ");
    Serial.println(axis);
    Serial.print("Input: ");
    Serial.println(inputStr);

    if(joystick == '1'){
      if(axis == '1'){
        input *= -1;
        analogWrite(R,floatTOint(input));
        analogWrite(Y,floatTOint(input));
        analogWrite(G,floatTOint(input));
        analogWrite(B,floatTOint(input));
      }
      else if(axis == '2'){
        if(input > 0){
          digitalWrite(Y,LOW);
          digitalWrite(R,HIGH);
          digitalWrite(G,HIGH);
          digitalWrite(B,LOW);
        }
        else if(input < 0){
          digitalWrite(Y,HIGH);
          digitalWrite(R,LOW);
          digitalWrite(G,LOW);
          digitalWrite(B,HIGH);
        }
      }
    }
    else if(joystick == '2'){
      if(axis == '1'){
        if(input < 0){
          digitalWrite(B,HIGH);
          digitalWrite(R,HIGH);
          digitalWrite(Y,LOW);
          digitalWrite(G,LOW);
        }
        else if(input > 0){
          digitalWrite(B,LOW);
          digitalWrite(R,LOW);
          digitalWrite(Y,HIGH);
          digitalWrite(G,HIGH);
        }
      }
      else if(axis == '2'){
        if(input < 0){
          digitalWrite(B,LOW);
          digitalWrite(R,HIGH);
          digitalWrite(Y,HIGH);
          digitalWrite(G,LOW);
        }
        else if(input > 0){
          digitalWrite(B,HIGH);
          digitalWrite(R,LOW);
          digitalWrite(Y,LOW);
          digitalWrite(G,HIGH);
        }
      } 
    }
  }
  else {
    Serial.println("----BUTTON INPUT----");
    Serial.print("command: ");
    Serial.println(command);
  }
}

void readInput_Float(char command, char joystick, char axis, float input){
  flash();
  int isConnect;
  int light = 0;
  static float t = 0; // Throttle [0,1]
  static float y = 0; // Yaw [-1,1]
  static float r = 0; // Roll [-1,1]
  static float p = 0; // Pitch [-1,1]
  static float wT = 0;
  unsigned long  timeNow = millis();
  int conStat = 0;
  String response = " ";
      
  if(command == '!'){
    // Change Control Values
    if(joystick == '1'){
      if(axis == '1'){
        input *= -1;
        t = input;
        flash();
      }
      else if(axis == '2'){
        y = input;
        flash();
      }
    }
    else if(joystick == '2'){
      if(axis == '1'){
        input *= -1;
        p = input;
        flash();
      }
      else if(axis == '2'){
        input *= -1;
        r = input;
        flash();
      }
    } 
    conStat = 0;
  }
  else if(command == 'a'){
    flash();
    conStat = 0;
  }
  else if(command == 'b'){
    if(light == 0){
      light = lightON();
    }
    else if(light == 1){
      light = lightOFF();
    }
    conStat = 0;
  }
  timeNow = millis();

//  wT = motor_write(t,y,r,p,wT);
      
}

int floatTOint(float input){
  int result = round(255*input);
  return result;
}

struct cmdline readRadioBuff(){
  if(radio.available()){
    while(radio.available()){
      radio.read(&buff, radio.getPayloadSize());
//      Serial.print("This just in ... ");
//      Serial.println(buff);
//      flash();
      struct cmdline cmd;
      byte ndx = 0;
      byte phase = 1;
      byte inByte;
      char command = 0;
      char joystick = 0;
      char axis = 0;
      int inc = 0;
      char inputStr[10] = "0.00";
      float input = 0;
      int negInput = 0;
       
      while(phase != 5){
    
        
        // Read a single byte from the buffer
        
        inByte = buff[ndx];
//        flash();
//        Serial.print("recieved byte: ");
//        Serial.println(char(inByte));
        

        if(char(inByte) == ' '){
          ndx++;
          continue;
        }
        else if(char(inByte) == '<'){
          phase = 5; // Error State, Command will not write to motor
          break;
        }



        // Determine data processing phase
        
        if(phase == 1){                       // Process Command Char
          command = char(inByte);
          ndx++;
          
          if(command != '!'){
            if(command == 'a' || command == 'b'){
              phase = 5;
            }
            else {
              phase = 1;
            }
          }
          else {
            phase = 2;
            negInput = 0;
          }
        }
        else if(phase == 2){                  // Process Joystick Char
          joystick = char(inByte);
          ndx++;
          
          if(joystick != '1' && joystick != '2'){
            phase = 1; 
          }
          else {
            phase = 3;
          }
        }
        else if(phase == 3){                  // Process Axis Char
          axis = char(inByte);
          ndx++;

          if(axis != '1' && axis != '2'){
            phase = 1; 
          }
          else {
            phase = 4;
          }
        }
        else if(phase == 4){                  // Process Input Float
          if(char(inByte) == '?'){
            inputStr[ndx] = '\0';
            input = String(inputStr).toFloat();
            ndx = 0;
            if(negInput == 1){
              input = input * -1;
            }
            phase = 5;
          }
          if(char(inByte) == '-'){
            ndx++;
            negInput = 1;
          }
          else {
            inputStr[inc] = char(inByte);
            ndx++;
            inc++;
          }
        }
      }

      cmd.command = command;
      cmd.joystick = joystick;
      cmd.axis = axis;
      cmd.inputStr = inputStr;
      cmd.input = input;

      return cmd;
      
//      printInput(command, joystick, axis, inputStr, input);
//      readInput_Float(command, joystick, axis, input);
    }
  }
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

int lightON(){
  int light = 1;
  digitalWrite(R,HIGH);
  digitalWrite(G,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(Y,HIGH);
  return light;
}

int lightOFF(){
  int light = 0;
  digitalWrite(R,LOW);
  digitalWrite(G,LOW);
  digitalWrite(B,LOW);
  digitalWrite(Y,LOW);
  return light;
}

