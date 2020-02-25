import serial
import numpy as np
import threading
# import time
import csv
import time

fieldnames = ["FLujo,Presión"]

def convert(s):
    # initialization of string to ""
    new = ""

    # traverse in the string
    for x in s:
        new += x

        # return string
    return new

try:
    s1 = serial.Serial(port="COM4", baudrate=9600)
    print("conexion hecha")
except:
    print("Error de conexion")

s1.flushInput()

def trama():
    arreglo_recibido1 = []
    a = 0
    while True:
        if s1.inWaiting()>0:
            try:
                inputValue = s1.read(1).decode('ascii')
            except:
                print("error")

            if inputValue == '[':
                arreglo_recibido1 = []
            elif inputValue == ']':
                arreglo_recibido2 = convert(arreglo_recibido1)
                #print(type(arreglo_recibido2))
                return arreglo_recibido2
                break
            else:
                arreglo_recibido1.append(inputValue)

with open('lectura4.csv', 'w') as csv_file:
    csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames, delimiter='\n')
    csv_writer.writeheader()
#timeout = time.time() + 60*2
while True:
    with open('lectura4.csv', 'a',newline='') as csv_file:
        csv_writer=csv.DictWriter(csv_file, fieldnames=fieldnames, delimiter='\n')
        #print(trama())
        info = {
            "FLujo,Presión": trama()
        }
        csv_writer.writerow(info)

