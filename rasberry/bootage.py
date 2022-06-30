import RPi.GPIO as gpio
import handgesture

def fonc2():
    buttonpin = 27
    buttonpout = 22

    gpio.setmode(gpio.BCM)
    gpio.setup(buttonpin,gpio.IN , pull_up_down=gpio.PUD_UP)
    gpio.setup(buttonpout,gpio.OUT)

    while True:
        if gpio.input(buttonpin) :
            return 1
        
if fonc2() == 1:
    handgesture.fonc1()
        
            
