// Motor Pins
int Y = 5;
int B = 4;
int G = 3;
int R = 2;

float e = 0.5; // Throttle Fraction
float f = 0.2; // Yaw Fraction
float g = 0.15; // Roll Fraction
float h = 0.15; // Pitch Fraction

// Quad Copter Flight Controller Simulation
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
  
void setup() {
  // set pin modes
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Y, OUTPUT);

  // init
  Serial.begin(9600);

  flash();
}

void loop() {
 readInput_Float();
}

float readInput_Float(){
  int joystick;
  int axis;
  float input;
  int command;
  int isConnect;
  int light = 0;
  float t = 0; // Throttle [0,1]
  float y = 0; // Yaw [-1,1]
  float r = 0; // Roll [-1,1]
  float p = 0; // Pitch [-1,1]
  float Tarray[] = {0,0}; // Current Throttle {wT,dwT}
  float wT = 0;
  unsigned long  timeNow = millis();
  int conStat = 0;
  String response = " ";
  
  
  while(1){   
     if(Serial.available()){
  
      command = Serial.read();
      
      if(command == '<'){
        joystick = Serial.parseInt();
        axis = Serial.parseInt();
        input = Serial.parseFloat();
  
        if(Serial.read() == '>'){
          // Change Control Values
          if(joystick == 1){
            if(axis == 1){
              input *= -1;
              t = input;
            }
            else if(axis == 2){
              y = input;
            }
          }
          else if(joystick == 2){
            if(axis == 1){
              input *= -1;
              p = input;
            }
            else if(axis == 2){
              input *= -1;
              r = input;
            }
          } 
          conStat = 0;
        }
      }
      else if(command == 97){
        flash();
        conStat = 0;
      }
      else if(command == 98){
        if(light == 0){
          light = lightON();
        }
        else if(light == 1){
          light = lightOFF();
        }
        conStat = 0;
      }
      else if(command == 45){
        response = Serial.readStringUntil('-');
        if(response == "hi"){
          flash();
          conStat = 0;
        }
      }
      timeNow = millis();
    }
    else if(!Serial.available() && conStat == 1){
      if(millis()-timeNow > 2000){
        while(1){
          flash();
          delay(100);
        }
      }
    }
    else if(!Serial.available() && conStat == 0){
      if(millis()-timeNow > 5000){
        Serial.write("hello");
        conStat = 1;
        timeNow = millis();
      }
    }
    wT = motor_write(t,y,r,p,wT);
    
  }
}

// Functions

int floatTOint(float input){
  int result = round(255*input);
  return result;
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
//
//void conCheck(){
//  
//  String response = " ";
//  
//  if(Serial.available()){
//    response = Serial.readString();
//  }
//  if(response == "hi"){
//    flash();
//  
//  }
//}

//void motorOutput(int joystick,int axis,float input,float t,float y,float r,float p){
// 
//  
//  // Define Control Constants
//  float e = 0.5; // Throttle Fraction
//  float f = 0.2; // Yaw Fraction
//  float g = 0.15; // Roll Fraction
//  float h = 0.15; // Pitch Fraction
//  
//  // Define Output Array
//  float omega[4];
//  float omega_yaw[4];
//  float omega_roll[4];
//  float omega_pitch[4];
//  int omega_int[4];
//
//  // Change Control Values
//  if(joystick == 1){
//    if(axis == 1){
//      t = input;
//    }
//    else if(axis == 2){
//      y = input;
//    }
//  }
//  else if(joystick == 2){
//    if(axis == 1){
//      input *= -1;
//      p = input;
//    }
//    else if(axis == 2){
//      r = input;
//    }
//  }
//  
//  // Control Equations
//  omega[4] = alt_ctrl(t,e);
//
////  omega_yaw[4] = yaw_ctrl(y, f, omega);
////  omega_roll[4] = roll_ctrl(r, g, omega);
////  omega_pitch[4] = pitch_ctrl(p, h, omega);
//
////  for(int x = 0; x < 4; x++){
////  omega[x] = omega_yaw[x] + omega_roll[x] + omega_pitch[x];
////  }
//
//  // Convert float to int
//  omega_int[0] = floatTOint(omega[0]);
//  omega_int[1] = floatTOint(omega[1]);
//  omega_int[2] = floatTOint(omega[2]);
//  omega_int[3] = floatTOint(omega[3]);
//
//  analogWrite(R,omega_int[0]);
//  analogWrite(B,omega_int[1]);
//  analogWrite(Y,omega_int[2]);
//  analogWrite(G,omega_int[3]);
//}



