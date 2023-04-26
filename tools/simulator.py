"""
Simulator for simulating MQTT API. It publishes messages with the same format to the same topics
as the MQTT telemetry plugin.
"""

import dearpygui.dearpygui as dpg
import json
import paho.mqtt.client as mqtt

height = 200
width = 300

class Simulator:
    def __init__(self, host:str = "localhost", port:int =1883):
        self.client = mqtt.Client()
        self.host = host
        self.port = port

    def publish(self, topic: str, data: dict):
        payload = json.dumps(data)
        self.client.publish(topic, payload)

    def start_ats(self):
        self.publish("trucksim/gameinfo", {
            "game_id": "ats",
            "game_version": 147
        })

    def start_ets2(self):
        self.publish("trucksim/gameinfo", {
            "game_id": "eut2",
            "game_version": 147
        })

    def goodbye(self):
        self.publish("trucksim/gameinfo", None)

    def start(self):
        self.client.connect(self.host, self.port, 60)

        dpg.create_context()
        dpg.create_viewport(title="Simulator", width=int(width * 1.1), height=int(height * 1.1), resizable=False)
        dpg.setup_dearpygui()

        with dpg.window(label="Main", width=width, height=height, no_resize=True, no_move=True, no_close=True):
            dpg.add_button(label="Start ATS", callback=self.start_ats)
            dpg.add_button(label="Start ETS2", callback=self.start_ets2)
            dpg.add_button(label="Goodbye", callback=self.goodbye)

        dpg.show_viewport()
        dpg.start_dearpygui()
        dpg.destroy_context()

if __name__ == "__main__":
    sim = Simulator()
    sim.start()