import time
import board
import neopixel
import RPi.GPIO as GPIO
from omxplayer.player import OMXPlayer
a = 1

GPIO.setmode(GPIO.BCM) 
GPIO.setup(6, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #первая свеча
GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)#вторая свеча
GPIO.setup(19, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)#третья свеча
GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)#четвертая свеча
GPIO.setup(20, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)#вход с цк
GPIO.setup(5, GPIO.OUT)                            #выход на цк
GPIO.output(5, GPIO.LOW)
pixels = neopixel.NeoPixel(board.D21, 4)           #21 пин - лента
pixels.show()
player = OMXPlayer('/video/30sec.mp4')

while GPIO.input(20)==0:
    if player.position()>6: player.set_position(0.0)
player.set_position(17.1)
time.sleep(0.5)
player.pause()
while True:
    if GPIO.input(6)==1:
        pixels[0] = (255, 107, 0)
        pixels.show()
        a = 2
        while a > 1:
            if GPIO.input(13)==1:
                pixels[1] = (255, 107, 0)
                pixels.show()
                a = 3
                while a > 2:
                    if GPIO.input(19)==1:
                        pixels[2] = (255, 107, 0)
                        pixels.show()
                        while True:
                            if GPIO.input(26)==1:
                                pixels[3] = (255, 107, 0)
                                pixels.show()
                                GPIO.output(5, GPIO.HIGH)
                                player.set_position(25)
                                player.play()
                                time.sleep(5)
                                player.pause()
                                while True:
                                    a = 4
                    if GPIO.input(26)==1:
                        a = 0
            if GPIO.input(19)==1 or GPIO.input(26)==1:
                a = 0
    if GPIO.input(13)==1 or GPIO.input(19)==1 or GPIO.input(26)==1 or a==0:
        pixels[0] = (0, 0, 0)
        pixels[1] = (0, 0, 0)
        pixels[2] = (0, 0, 0)
        pixels[3] = (0, 0, 0)
        pixels.show()
        player.play()
        while player.position()<20:
            a = 1
        player.set_position(17)
        time.sleep(0.2)
        player.pause()
        time.sleep(0.2)
