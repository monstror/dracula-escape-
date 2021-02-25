import RPi.GPIO as GPIO
import time
from omxplayer.player import OMXPlayer

flag = 0

GPIO.setmode(GPIO.BCM)
GPIO.setup(25, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(8, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(7, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(1, GPIO.OUT)
GPIO.output(1, GPIO.LOW)

player = OMXPlayer('/video/30sec.mp4')
time.sleep(0.5)

while GPIO.input(7)==0:
    if player.position()>5: player.set_position(0)

while True:
    if GPIO.input(25)==1:
        print("GPIO 25 1")
        if player.position()<5:
            player.play()
            print("play")
        else:
            print("pause")
            player.pause()
            while GPIO.input(25)==1:
                print("GPIO 25 1 wait 8 and 7")
                time.sleep(0.5)
                if GPIO.input(25)==1 and GPIO.input(8)==1 and GPIO.input(7)==1:
                    if player.position()>10.0:
                            player.pause()
                            flag = 1
                            GPIO.output(1, GPIO.HIGH)
                    else:
                        player.play()
    elif GPIO.input(25)==0:
        print("GPIO 25 0")
        if flag == 1:
            while True:
                GPIO.output(1, GPIO.HIGH)
        if GPIO.input(25)==0:
            print("RESTART! RESTART! RESTART!")
            player.set_position(0)
            player.play()
            time.sleep(0.5)
            player.pause()
