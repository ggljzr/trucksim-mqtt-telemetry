"""
Listens to plugin LOG topic and prints messages.

Connects to localhost on port 1883.

Usage: python logger.py
"""

import json
import logging

import paho.mqtt.client as mqtt

logging.basicConfig(
    format='%(asctime)s %(levelname)-8s %(message)s',
    level=logging.DEBUG,
    datefmt='%Y-%m-%d %H:%M:%S')

LOG_TOPIC = "trucksim/log"


def on_connect(client: mqtt.Client, userdata, flags, rc):
    logging.info(f"Connected with result code: {rc}")
    logging.info(f"Subscribing topic {LOG_TOPIC}")
    client.subscribe(LOG_TOPIC)

def on_message(client: mqtt.Client, userdata, msg):
    data = json.loads(msg.payload)
    logging.log(**data)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("127.0.0.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
