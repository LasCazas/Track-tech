#!/usr/bin/env python3

# Import the necessary libraries
import time
import math
from ev3dev2.motor import *
from ev3dev2.sound import Sound
from ev3dev2.button import Button
from ev3dev2.sensor import *
from ev3dev2.sensor.lego import *
from ev3dev2.sensor.virtual import *

# Create the sensors and motors objects
motorA = LargeMotor(OUTPUT_A)
motorB = LargeMotor(OUTPUT_B)
left_motor = motorA
right_motor = motorB
tank_drive = MoveTank(OUTPUT_A, OUTPUT_B)
steering_drive = MoveSteering(OUTPUT_A, OUTPUT_B)

spkr = Sound()
btn = Button()
radio = Radio()

color_sensor_in1 = ColorSensor(INPUT_1)
ultrasonic_sensor_in2 = UltrasonicSensor(INPUT_2)
gyro_sensor_in3 = GyroSensor(INPUT_3)
gps_sensor_in4 = GPSSensor(INPUT_4)
pen_in5 = Pen(INPUT_5)
color_sensor_in6 = ColorSensor(INPUT_6)
color_sensor_in7 = ColorSensor(INPUT_7)

motorC = LargeMotor(OUTPUT_C) # Magnet

# Declaração de Constantes
DELTAPULSO = 11  # Pesquisei e deu 11 pulsos por volta
Branco = 0
Preto = 1
Kp = 8  # Variavel que multiplica o erro da posicao do carro para o ajuste fino
Ki = 2
Kd = 1
erro = 0
erroA = 0
Quebra = 20
while True:
    SenOE = Preto
    SenE = color_sensor_in1.reflected_light_intensity
    SenC = color_sensor_in6.reflected_light_intensity
    SenD = color_sensor_in7.reflected_light_intensity
    SenOD = Preto
    if SenC > Quebra:
        SenC = Preto
    else:
        SenC = Branco
    if SenE > Quebra:
        SenE = Preto
    else:
        SenE = Branco
    if SenD > Quebra:
        SenD = Preto
    else:
        SenD = Branco
    if SenOE == Preto and SenE == Preto and SenC == Branco and SenD == Preto and SenOD == Preto:
        erro = 0
    elif SenOE == Preto and SenE == Branco and SenC == Branco and SenD == Preto and SenOD == Preto:
        erro = -1
    elif SenOE == Preto and SenE == Branco and SenC == Preto and SenD == Preto and SenOD == Preto:
        erro = -1.5
    elif SenOE == Branco and SenE == Preto and SenC == Preto and SenD == Preto and SenOD == Preto:
        erro = -2
    elif SenOE == Preto and SenE == Preto and SenC == Branco and SenD == Branco and SenOD == Preto:
        erro = 1
    elif SenOE == Preto and SenE == Preto and SenC == Preto and SenD == Branco and SenOD == Preto:
        erro = 2
    if erro == 0:
        I = 0
    P = erro * Kp
    I = I + erro
    if I > 100:
        I = 100
    elif I < -100:
      I = -100
    D = erro - erroA
    PID = P + (Ki * I) + (Kd * D)
    
    erroA = erro
    if PID >= 0:   
        VeloE = 20 + PID
        VeloD = 20 - PID
    else:   
        VeloE = 20 + PID
        VeloD = 20 - PID
    motorA.on(VeloE)
    motorB.on(VeloD)