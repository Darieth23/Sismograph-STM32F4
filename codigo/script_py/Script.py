#!/usr/bin/python3
import serial
import paho.mqtt.client as mqtt
import json
#Functions
def on_publish(client,userdata,result):             #create function for callback
    print("data published to thingsboard \n")
    pass
#Serial connection
ser = serial.Serial(
    port='/dev/ttyACM0',\
    baudrate=115200,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=200\
)
print("Connected to: " + ser.portstr)
#Datos para la conexión con el cliente:
line = []
dictionary = dict()
broker = "iot.eie.ucr.ac.cr"
port = 1883
topic = "v1/devices/me/telemetry"
username = "STM"
password = "ozwnzj5hfdmn1nyjlxev"
data_header = ["x", "y", "z", "Battery"]
print(data_header)
#Establecemos los parámetros de conexión
client = mqtt.Client(username)
client.on_publish = on_publish
client.username_pw_set(password)
client.connect(broker, port, keepalive=60)
while True:
    data_captured = ser.readline().decode().strip().split(",")
    print(data_captured)
    dictionary["x"] = data_captured[0]
    dictionary["y"] = data_captured[1]
    dictionary["z"] = data_captured[2]
    if data_captured[3] == '1':
        alarm = "Yes"
    else:
        alarm = "No"
    dictionary["Battery Low"] = alarm
    dictionary["Battery Level"] = data_captured[4]
    output = json.dumps(dictionary)
    print(output)
    client.publish(topic, output)
ser.close()