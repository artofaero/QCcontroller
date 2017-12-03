#!/usr/bin/env python

import serial
import pygame
import time
import math

# Define class that converts text to binary
# def text_to_bits(text, encoding='utf-8', errors='surrogatepass'):
	# bits = bin(int.from_bytes(text.encode(encoding, errors), 'big'))[2:]
	# return bits.zfill(8 * ((len(bits) + 7) // 8))

# Open USB Port
ser = serial.Serial()
ser.baudrate = 250000
ser.port = 'COM4'
ser.timeout = 0
# ser.write_timeout = 0.1
ser
ser.open()
time.sleep(2) # Wait for Arduino to reset
if ser.is_open == True:
	print('\nSerial connection with Arduino established!\n')

# init controller
pygame.init()
controller = pygame.joystick.Joystick(0)
controller.init()
print('Xbox Controller Connected\n')


print('/**************************/')
print('Joystick Drive Program')
print("Press Start Button to quit")
print('/**************************/\n')

# Create Display Surface
# screen_width=700
# screen_height=400
# screen=pygame.display.set_mode([screen_width,screen_height])
# pygame.display.set_caption('Xbox Controller Input')

# font = pygame.font.Font(None, 36)
# text = font.render("Hello There", 1, (10, 10, 10))
# textpos = text.get_rect()
# textpos.centerx = background.get_rect().centerx
# background.blit(text, textpos)

def write_ctrl(xIn, deadzone, scale_factor, joystick, axis):
	# xIn = control output from joystick
	# deadzone = joystick output limit that will pass an ouput of 0 to the program
	# scale_factor = Used to scale ouptput after deadzone threshold has been passed
	# joystick = Identifies which joystick is ouputting. String of 1 or 2.
	# axis = Identifies which axis is outputting. String of 1 or 2.
	
	if math.fabs(xIn) <= deadzone:
		xIn = 0
	else:
		xIn = (math.fabs(xIn) - deadzone)*scale_factor*(float(xIn)/math.fabs(xIn))
	xIn = str(round(xIn,3)).encode()
	# Write Left Joy, y-axis
	ser.write(b'<')
	ser.write(joystick.encode()) # Send Joystick Identifier
	ser.write(b' ') 
	ser.write(axis.encode()) # Send Axis Identifier
	ser.write(b' ')
	ser.write(xIn) # Send Output
	ser.write(b'>')

def write_button(button):
	# button = Identifies which button is outputting. String such as a, b, y, x, etc.
	ser.write(button.encode())
	# ser.write(b'>')
		

# Initialize Control Loop Variables
key = 0
yl = 0
xl = 0
yr = 0
xr = 0
deadzone = float(0.2)
scale_factor = float(1/(1-deadzone))
check = ' '
response = ' '
inBuff = 0

# Control Loop
while key != 'q':
	for event in pygame.event.get():
		if event.type == pygame.JOYAXISMOTION:
			if event.axis == 1: # Left Joystick, y-axis
				yl = event.value
				write_ctrl(yl, deadzone, scale_factor, '1', '1')
			# End
			if event.axis == 0: # Left Joystick, x-axis
				xl = event.value
				write_ctrl(xl, deadzone, scale_factor, '1', '2')
			# End
			if event.axis == 3: # Right Joystick, y-axis
				yr = event.value
				write_ctrl(yr, deadzone, scale_factor, '2', '1')
			# End
			if event.axis == 4: # Right Joystick, x-axis
				xr = event.value
				write_ctrl(xr, deadzone, scale_factor, '2', '2')
			# End
				
		# Press A to Flash
		if event.type == pygame.JOYBUTTONDOWN:
			if event.button == 0:
				write_button('a')
			# End
		# End
			
		# Press Start to Quit
		if event.type == pygame.JOYBUTTONDOWN:
			if event.button == 7:
				key = 'q'
			# End
		# End
		
		# Hold B to light board
		if event.type == pygame.JOYBUTTONDOWN:
			if event.button == 1:
				write_button('b')
			# End
		# End
		if event.type == pygame.JOYBUTTONUP:
			if event.button == 1:
				write_button('b')
			# End
		# End	
	# End	
	
	inBuff = ser.in_waiting
	if inBuff == 5:
		check = ser.read(5)
		check = check.decode("utf-8")
		print(check)
		
		if check == 'hello':
			ser.write(b'-')
			ser.write(b'hi')
			ser.write(b'-')
		# End
	# End
	
# End control loop
	


ser.close()
if ser.is_open == False:
	print('Serial connection with Arduino terminated!')