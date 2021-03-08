import paho.mqtt.client as mqtt
import time

import serial

# ser = serial.Serial('COM6', 115200, timeout=0.3)  # Establish the connection on a specific port

topic_pot_led = "AmbInt/sensors/pot_led"     # Potenciometer Topic
topic_temp_led = "AmbInt/sensors/temp_led"   # Temperature Topic
topic_int_led = "AmbInt/sensors/int_led"     # Intensity Topic


def on_message(client, userdata, message):
    print("message received ", str(message.payload.decode("utf-8")))
    print("message topic=", message.topic)
    # sensor_name = message.topic.split('/')[-1]
    # ser.write((sensor_name + ":" + str(message.payload.decode("utf-8"))).encode())

    # ser_bytes = ser.readline()
    # decoded_bytes = ser_bytes.decode("utf-8")
    # if decoded_bytes:
    #     led_status = int(decoded_bytes)
    #     print("Publishing", led_status, "to", sensor_name, "LED Topic AmbInt/sensors/"+sensor_name+"_led the value", led_status, "\n")
    #     client.publish("AmbInt/sensors/"+sensor_name+"_led", led_status)

    time.sleep(0.1)


broker_address="broker.emqx.io"
print("creating new instance")
client = mqtt.Client("ACIC6_Receiver") #create new instance
client.on_message = on_message #attach function to callback

print("connecting to broker")
client.connect(broker_address) #connect to broker
client.loop_start() #start the loop

print("Subscribing to topic", "AmbInt/sensors/pot")
client.subscribe("AmbInt/sensors/pot")
print("Subscribing to topic", "AmbInt/sensors/int")
client.subscribe("AmbInt/sensors/int")
print("Subscribing to topic", "AmbInt/sensors/temp")
client.subscribe("AmbInt/sensors/temp")

time.sleep(1000)


# client.loop_stop()  # stop the loop

