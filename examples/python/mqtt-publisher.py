"""
Basic MQTT publisher with Paho MQTT client.

Connects to localhost on port 1883.

Usage: python mqtt-subscriber.py topic msg

Publishes given message and exits.
"""

from sys import argv

import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client: mqtt.Client, userdata, flags, rc):
    print("Connected with result code " + str(rc))


client = mqtt.Client()
client.on_connect = on_connect

client.connect("127.0.0.1", 1883, 60)
topic, msg = argv[1:]

print(f"Publishing message {msg} on topic {topic}...")
msg_info = client.publish(topic, msg)

print("Done!")
print(msg_info)
