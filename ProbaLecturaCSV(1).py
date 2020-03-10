#!/usr/bin/env python
# coding: utf-8

import matplotlib.pyplot as plt
import csv

datos = [] #Creamos a lista para almacenar os datos

with open('proba.csv', newline='') as File:  #Abrimos o arquivo e creamos o punteiro
    reader = csv.reader(File) #Creamos e inicalizamos o "lector" para archivo csv
    for row in reader: #Almacenamos a información na lista
        datos.append(row)
        
#Adaptamos os datos para facer o plot
x = []
y = []
for i in range(len(datos)):
    x.append(datos[i][0])
    y.append(datos[i][1])
        
plt.plot(x ,y, 'ro')
plt.axis([0, 20, 0, 20])
plt.show()





