import serial
import time
import sys
from tkinter import *
from time import sleep
volt_POT1 = 0.00
volt_POT2 = 0.00
real_POT1 = 0	
real_POT2 = 0	
gui = Tk()
Text=StringVar()
serialPort = serial.Serial(port = 'com10', baudrate = 9600,
                    parity = serial.PARITY_NONE,
                    stopbits = serial.STOPBITS_ONE,
                    bytesize = serial.EIGHTBITS, timeout = 0)
    
#interfaz grafica    


gui.title("Lab4. Comunicacion Serial SPI y UART")
nombre = Label(gui, text ="Juan Diego Castillo Amaya \n17074", fg = 'white', bg = 'blue')
nombre.grid (row = 1, column = 2)
count = Label(gui, text = 'Valor a enviar', fg='white', bg='green')
count.grid (row = 3, column = 2)
label = Label(gui, textvariable=Text)
label.grid(row = 6, column = 3)
Volt1 = Label (gui, text = 'Pot1:', fg='white', bg='orange')
Volt1.grid(row = 8 ,column = 1)
Volt2 = Label (gui, text = 'Pot2:', fg='white', bg='orange')
Volt2.grid(row = 8 ,column = 3)
#espacios
Espacio1 = Label (gui, text ='')
Espacio1.grid (row = 2, column = 2)
Espacio2 = Label (gui, text ='')
Espacio2.grid (row = 4, column = 2)
Espacio3 = Label (gui, text ='')
Espacio3.grid (row = 6, column = 2)

#Espacio para colocar valor
Numero = StringVar(value='')
caja = Entry(gui, textvariable=Numero)
caja.grid(row = 5, column=1)

#funcion de enviar valor
def enviar():
    serialPort.flushInput()
    serialPort.flushOutput()
    time.sleep(.3)
    num = Numero.get()
    print(num)
    print(type(num))
    while(1):
        try:
            valor=float(num)
            break
        except:
            text.set('ingrese valor de 0 a 255')
            label.config(fg='red')
            return
    if(not(float(num)>=0 and float(num)<=255)):
        Text.set('ingrese valor de 0 a 255')
        label.config(fg='red')
    else:
        Text.set('enviado')
        label.config(fg='green')
        valor=int(num)
        number=int((valor/180)*33)+5
        valor=bytes([number])
        serialPort.write(valor)
        
#boton
boton1 = Button(gui, text='enviar', fg = 'white', bg='blue', command=enviar)
boton1.grid (row = 5 , column = 3)
POT_1 = Label(gui,text=str(volt_POT1) ,fg="white", bg = 'orange')
POT_1.grid(row=9,column = 1)    
POT_2 = Label(gui,text=str(volt_POT2) ,fg="white",bg = 'orange')	
POT_2.grid(row = 9, column = 3)
#While loop en el que se mostraran los datos de los potenciometros enviados del PIC en la computadora	#While loop en el que se mostraran los datos de los potenciometros enviados del PIC en la computadora
while True:	
    dato = serialPort.read()	    
    try:	   
        if dato == '':	        
            pass	            
        else:	        
            dato_POT1 = serialPort.read()	            
            serialPort.flushOutput()	            
            dato_POT2 = ser.read()	            
            real_POT1 = ord(dato_POT1)	            
            real_POT2 = ord(dato_POT2)	            
            print(str(real_POT1) + ',' + str(real_POT2))	            
            serialPort.flushInput()
            time.sleep(.05)
            	            
    except:	   
        print('#')	        
    volt_POT1 = float((real_POT1)/float (51))
    volt_POT2 = float((real_POT2)/float (51))
    POT_1.config(text = str(volt_POT1)[0:4])
    POT_2.config(text = str(volt_POT2)[0:4])
    gui.update_idletasks()	    
    gui.update() 	    
    time.sleep(.2)	   

while(1):
        
    serialPort.flushInput()
    serialPort.flushOutput()
    time.sleep(.3)
    recibido1=serialPort.read()
    #ser.write(recibido1)
    Volt1 = round((ord(recibido1)*5)/255,2)
    label2=Label(gui, text = Volt1)
    label2.grid(row=9, column=1)
    gui.update()
