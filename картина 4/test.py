import RPi.GPIO as GPIO
import time
from omxplayer.player import OMXPlayer

GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #первый датчик
GPIO.setup(24, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #второй датчик
GPIO.setup(15, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #вход с цк
GPIO.setup(14, GPIO.OUT)                            #выход на цк
GPIO.output(14, GPIO.LOW)

player = OMXPlayer('/video/30sec.mp4')

while GPIO.input(15)==0:
    if player.position()>6: player.set_position(0.0)
player.set_position(15)
time.sleep(0.5)
player.pause()

while GPIO.input(23)==1 or GPIO.input(24)==1:
    a = 1

player.play()
time.sleep(2)
player.pause()

while True:
    GPIO.output(14, GPIO.HIGH)
