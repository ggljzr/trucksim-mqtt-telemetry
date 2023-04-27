"""
Simulator for simulating MQTT API. It publishes messages with the same format to the same topics
as the MQTT telemetry plugin.
"""

from typing import Any

import dearpygui.dearpygui as dpg
import json
import paho.mqtt.client as mqtt

height = 400
width = 400


class Simulator:
    def __init__(self, host: str = "localhost", port: int = 1883):
        self.client = mqtt.Client()
        self.host = host
        self.port = port

    def publish(self, topic: str, data: dict):
        payload = json.dumps(data)
        self.client.publish(topic, payload)

    def start_ats(self):
        self.publish("trucksim/gameinfo", {"game_id": "ats", "game_version": 147})

    def start_ets2(self):
        self.publish("trucksim/gameinfo", {"game_id": "eut2", "game_version": 147})

    def goodbye(self):
        self.publish("trucksim/gameinfo", None)

    def set_job(self):
        job = {
            "cargo": "Space Container",
            "cargo_id": "space_cont",
            "cargo_loaded": 1,
            "cargo_mass": 12020.2001953125,
            "cargo_unit_count": 1,
            "cargo_unit_mass": 12020.2001953125,
            "delivery_time": 100135,
            "destination_city": "Raton",
            "destination_city_id": "raton",
            "destination_company": "Rail Export",
            "destination_company_id": "re_train",
            "income": 66445,
            "is_special_job": 0,
            "job_market": "quick_job",
            "planned_distance_km": 1375,
            "source_city": "Houston",
            "source_city_id": "houston",
            "source_company": "Space Center",
            "source_company_id": "spc_whs",
        }
        self.publish("trucksim/event/config/job", job)

    def set_truck(self):
        truck = {
            "adblue_capacity": 80.0,
            "adblue_warning_factor": 0.15000000596046448,
            "battery_voltage_warning": 11.880000114440918,
            "brake_air_pressure_emergency": 29.579999923706055,
            "brake_air_pressure_warning": 59.15999984741211,
            "brand": "Kenworth",
            "brand_id": "kenworth",
            "cabin_position": {"x": 0.0, "y": 3.0, "z": -2.0},
            "differential_ratio": 3.25,
            "forward_ratio": 0.7300000190734863,
            "fuel_capacity": 833.0,
            "fuel_warning_factor": 0.15000000596046448,
            "gears_forward": 18,
            "gears_reverse": 4,
            "head_position": {
                "x": -0.42330673336982727,
                "y": -0.5754554271697998,
                "z": 0.3827627897262573,
            },
            "hook_position": {"x": 0.0, "y": 1.0, "z": 3.594026803970337},
            "id": "vehicle.kenworth.w900",
            "license_plate": "R56-9388",
            "license_plate_country": "Texas",
            "license_plate_country_id": "texas",
            "name": "W900",
            "oil_pressure_warning": 10.149999618530273,
            "retarder_steps": 3,
            "reverse_ratio": -3.430000066757202,
            "rpm_limit": 2100.0,
            "water_temperature_warning": 105.0,
            "wheel_position": {
                "x": 0.960354208946228,
                "y": 0.5059999823570251,
                "z": 4.1931939125061035,
            },
            "wheel_powered": 1,
            "wheel_radius": 0.5072842240333557,
            "wheel_simulated": 1,
            "wheel_steerable": 0,
            "wheels_count": 6,
        }
        self.publish("trucksim/event/config/truck", truck)

    def unset_job(self):
        self.publish("trucksim/event/config/job", None)

    def unset_truck(self):
        self.publish("trucksim/event/config/truck", None)

    def publish_channel_value(self, topic: str, value: Any):
        self.publish(topic, {"value": value})

    def start(self):
        self.client.connect(self.host, self.port, 60)

        dpg.create_context()
        dpg.create_viewport(
            title="Simulator",
            width=int(width),
            height=int(height * 1.1),
            resizable=False,
        )
        dpg.setup_dearpygui()

        with dpg.window(
            label="Presets",
            width=width,
            height=height * 0.3,
            no_resize=True,
            no_move=True,
            no_close=True,
        ):
            with dpg.group(horizontal=True):
                dpg.add_button(label="Start ATS", callback=self.start_ats)
                dpg.add_button(label="Start ETS2", callback=self.start_ets2)
                dpg.add_button(label="Goodbye", callback=self.goodbye)

            with dpg.group(horizontal=True):
                dpg.add_button(label="Set Job", callback=self.set_job)
                dpg.add_button(label="Unset Job", callback=self.unset_job)

            with dpg.group(horizontal=True):
                dpg.add_button(label="Set Truck", callback=self.set_truck)
                dpg.add_button(label="Unset Truck", callback=self.unset_truck)

        def make_channel_control(label, topic):
            with dpg.group(horizontal=True):
                input_id = dpg.add_input_int(label=label, width=width * 0.3)
                dpg.add_button(
                    label="Set",
                    callback=lambda: self.publish_channel_value(
                        topic, dpg.get_value(input_id)
                    ),
                )

        with dpg.window(
            label="Channels",
            width=width,
            height=height * 0.7,
            pos=[0, height * 0.3],
            no_resize=True,
            no_move=True,
            no_close=True,
        ):
            make_channel_control(
                "Game time [minutres]",
                "trucksim/channel/game/time",
            )

            make_channel_control(
                "Next rest stop [minutes]",
                "trucksim/channel/rest/stop",
            )

            make_channel_control(
                "Navigation time [seconds]",
                "trucksim/channel/truck/navigation/time",
            )

            make_channel_control(
                "Navigation distance [meters]",
                "trucksim/channel/truck/navigation/distance",
            )

        dpg.show_viewport()
        dpg.start_dearpygui()
        dpg.destroy_context()


if __name__ == "__main__":
    sim = Simulator()
    sim.start()
