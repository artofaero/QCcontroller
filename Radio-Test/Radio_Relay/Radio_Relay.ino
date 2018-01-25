
#include <SPI.h>
#include "RF24.h"
/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
byte addresses[][6] = {"1Node","2Node"};

/****************** Data Handling Globals ***************/
const byte numChars = 32;
char buff[numChars];
boolean newData = false;
/********************************************************/

void setup() {
  Serial.begin(115200);
  
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
  
  // Get the radio ready to send data
  radio.stopListening();

}

void loop() {

readIntoBuff();
writeNewData();
//reportData();
}

void readIntoBuff(){
  static boolean readInProgress = false;
  static byte ndx = 0;
  char startMark = '<';
  char endMark = '>';
  char data;

  while(Serial.available() > 0 && newData == false) {
    data = Serial.read();

    if(readInProgress == true) {
      if(data != endMark){
        buff[ndx] = data;
        ndx++;
        if (ndx >= numChars){
          ndx = numChars - 1;
        }
      }
      else {
        buff[ndx] = '\0';
        readInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if(data == startMark){
      readInProgress = true;
    }
  }
}

void writeNewData(){
  if(newData == true){
    radio.write(&buff, sizeof(buff));
    newData = false;
    memset(buff, 0, sizeof(buff));
  }
}

void reportData(){
  if(newData == true){
    Serial.write(buff);
    newData = false;
  }
}


