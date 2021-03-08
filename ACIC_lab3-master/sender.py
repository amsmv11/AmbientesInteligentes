import sys
import time
from time import sleep
import datetime
import paho.mqtt.client as mqtt
import serial

prev_pot_health = 1
prev_temp_health = 1
prev_int_health = 1

def on_message(client, userdata, message):
    global prev_pot_health, prev_temp_health, prev_int_health

    led_status = str(message.payload.decode("utf-8"))
    topic_name = message.topic.split('/')[-1]
    sensor = topic_name.split('_')[0]
    
    value = ""
    # If LED is down and was previously up, write to file the change of status
    if(led_status == "0"):
        print("WARNING:", sensor, "LED is DOWN!")
        if(sensor == "pot" and prev_pot_health == 1):
            value = str(datetime.datetime.now()) + ' sensor\'s LED ' + sensor + ' DOWN\n'
            prev_pot_health = int(led_status)  # Update led status
        elif(sensor == "temp" and prev_temp_health == 1):
            value = str(datetime.datetime.now()) + ' sensor\'s LED ' + sensor + ' DOWN\n'
            prev_temp_health = int(led_status)  # Update led status
        elif(sensor == "int" and prev_int_health == 1):
            value = str(datetime.datetime.now()) + ' sensor\'s LED ' + sensor + ' DOWN\n'
            prev_int_health = int(led_status)  # Update led status
    # If LED is up and previously was down, write to file the change of status
    else:
        if(sensor == "pot" and prev_pot_health == 0):
            value = str(datetime.datetime.now()) + ' sensor\'s LED ' + sensor + ' is now UP and running\n'
            prev_pot_health = int(led_status)  # Update led status
        elif(sensor == "temp" and prev_temp_health == 0):
            value = str(datetime.datetime.now()) + ' sensor\'s LED ' + sensor + ' is now UP and running\n'
            prev_temp_health = int(led_status)  # Update led status
        elif(sensor == "int" and prev_int_health == 0):
            value = str(datetime.datetime.now()) + ' sensor\'s LED ' + sensor + ' is now UP and running\n'
            prev_int_health = int(led_status)  # Update led status

    # Write to file if there is something to write
    if(value != ""):
        f = open("led_status.log", "a")
        f.write(value)
        f.close()

    time.sleep(0.1)


# ser = serial.Serial('/dev/ttyACM0', 115200) # For ubuntu
# ser = serial.Serial('/dev/cu.usbmodem14301', 115200) # For MacOS
# ser.flushInput()

topic_pot = "AmbInt/sensors/pot"     # Potenciometer Topic
topic_temp = "AmbInt/sensors/temp"   # Temperature Topic
topic_int = "AmbInt/sensors/int"     # Intensity Topic

topic_pot_led = "AmbInt/sensors/pot_led"     # Potenciometer LED Fail Topic
topic_temp_led = "AmbInt/sensors/temp_led"   # Temperature LED Fail Topic
topic_int_led = "AmbInt/sensors/int_led"     # Intensity LED Fail Topic

broker_address = "broker.emqx.io"

print("Creating new client instance")
client = mqtt.Client("ACIC6_Sender") # Create new instance
client.on_message = on_message #attach function to callback

print("Connecting to broker\n")
client.connect(broker_address) # Connect to broker

print("Subscribing to fail topic", topic_pot_led)
client.subscribe(topic_pot_led)
print("Subscribing to fail topic", topic_temp_led)
client.subscribe(topic_temp_led)
print("Subscribing to fail topic", topic_int_led)
client.subscribe(topic_int_led)

sleep(1)

client.loop_start() # Start the loop
try:
    while True:
        try:
            # ser_bytes = ser.readline()
            # decoded_bytes = ser_bytes.decode("utf-8")
            # data = decoded_bytes.split(" ")

            temp_value = "32º"
            pot_value = "40W"
            int_value = 11

            print("Publishing", temp_value, "to Temperature Topic", topic_temp)
            client.publish(topic_temp, temp_value)

            print("Publishing", pot_value, "to Potentiometer Topic", topic_pot)
            client.publish(topic_pot, pot_value)

            print("Publishing", int_value, "to Intensity Topic", topic_int, "\n")
            client.publish(topic_int, int_value)

        except Exception as e:
            print("Error:", e)
            break

except KeyboardInterrupt:
    print(" Ending loop...")

client.loop_stop() # Stop the loop