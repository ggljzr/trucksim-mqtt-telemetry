"""
MQTT subscriber for topics that publish JSON messages.

Connects to localhost on port 1883.

Usage: python mqtt-subscriber.py topic
"""

from sys import argv
import json
import pprint
import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client: mqtt.Client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    print(f"Subscribing topic {argv[1]}")
    client.subscribe(argv[1])


# The callback for when a PUBLISH message is received from the server.
def on_message(client: mqtt.Client, userdata, msg):
    data = json.loads(msg.payload)
    print(f"Received message on topic {msg.topic}")
    pprint.pprint(data)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("127.0.0.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
