# QCcontroller
Quad Copter Control Code for Arduino Devices

The purpose of this project is to create a microcontroller for a Quad Copter (QC) that can be controlled from a PC.

Main Goals:
1. Capture control inputs from keyboard or gamepad
2. Transmit control inputs wirelessly to Quad Copter Microcontroller (QCM)
3. Write control feedback loops for QCM that translate inputs from PC into changes in pitch, roll, yaw, and altitude. 
   The QCM must be able to automatically stabilize the QC when no input is being recieved.
   
Current Setup:
  Hardware:
    Windows PC
    Xbox 360 gamepad
    Arduino UNO R3
    Arduino MEGA
    NRF24L01 2.4 GHz Tx/Rx
  
  Hardware Layout:
  Gamepad <---usb---> PC <---usb---> Arduino UNO R3 <(((NRF24L01)))> Arduino MEGA
