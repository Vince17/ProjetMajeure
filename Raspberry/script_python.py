import os
import time
import datetime
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected success")
    else:
        print(f"Connected fail with code {rc}")
    
    client.subscribe("gaz_sensor")
        
def on_message(client, userdata, msg):
    papa = str(msg.payload)
    print(papa)
    papaleft = papa.lstrip('b\'')
    paparight = papaleft.rstrip('\'')
    papaToInt = float(paparight)
    
    if(papaToInt != ""):
        print(msg.topic +" : ")
        print(papaToInt)
        yapetetrelefeudanlabarak(papaToInt)

def yapetetrelefeudanlabarak(papaToInt):
    if(papaToInt >= 400):
        now = datetime.datetime.now()
        time_picture = now.strftime("%Y-%m-%d_%H-%M-%S")
        print(time_picture)
        os.system('fswebcam -r 1280x720 /home/pi/Pictures_Raspb/'+time_picture+'.jpg')
        os.system('sh /home/pi/Pictures_Raspb/script '+time_picture+'.jpg')
        print('sleep 60sec')
        time.sleep(60)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)

client.loop_forever()
