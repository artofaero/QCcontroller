# QCcontroller
Quad Copter Control Code for Arduino Devices

The purpose of this project is to create a microcontroller for a Quad Copter (QC) that can be controlled from a PC.
This is a work in progress. The code has not been tested on an actual QC as of yet. The goals listed below must be achieved in simulations before real world testing will commence.

Main Goals:
1. Capture control inputs from keyboard or gamepad
2. Transmit control inputs wirelessly to Quad Copter Microcontroller (QCM)
3. Write control feedback loops for QCM that translate inputs from PC into changes in pitch, roll, yaw, and altitude. 
   The QCM must be able to automatically stabilize the QC when no input is being recieved.
4. If loss of wireless/wired connectivity at any point within the system occurs, a failsafe procedure shall be implemented that will stabilize the QC until connection is reestablished or ground the CQ if reconnection is not achieved.
   
Current Setup:
  Hardware:
    Windows PC
    Xbox 360 gamepad
    Arduino MEGA
    Breadboard
  
  Hardware Layout:
  Gamepad <---usb---> PC <---usb---> Arduino MEGA <---jumpers---> Breadboard

Required Libraries:
   Python:
      Pyserial https://pythonhosted.org/pyserial/pyserial.html
      Pygame www.pygame.org      

Features:
   V0.1
      xbox_ctrl_float.py
         -Capture Joystick and button inputs from Xbox 360 gamepad.
         -Output Joystick ID, axis ID, and float value of joystick event via serial connection to Arduino.
         -Output Button Press and Release events to Arduino.
         -Recieve and respond to periodic serial connection confirmation requests from Arduino when no data is being written to the                   serial port.
      Quad_Ctrl.ino
         -Recieve control inputs via serial port.
         -Translate control inputs from roll, pitch, and yaw joystick axes into PWM outputs that control the speed of each QC motor in order to produce the desired roll, pitch, and yaw. 
         -Throttle is held at a constant value. It can be adjusted at various rates dependent on the magnitude of the throttle input.
         -Motor outputs are simulated by LEDs arranged in the layout of QC motors. The brightness of the LED indicates the motor speed.
         -If no serial communication is received for a period of 5 seconds, the QCM will send a message to the PC. If neither the        expected response or any type of control input is recieved within 2 seconds, the QCM is sent into failsafe mode. Currently, the only failsafe procedure is to flash all four LEDs once.
         
      
         
