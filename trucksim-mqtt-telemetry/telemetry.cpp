#include "pch.h"

#include <stdio.h>

#include <mqtt/client.h>
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry_event.h"

#include "telemetry.h"
#include "topics.h"

namespace trucksim_mqtt {
	Telemetry::Telemetry(mqtt::client* client) : client(client) {}

	void Telemetry::log(const char* msg_text) {
		auto msg = mqtt::make_message(LOG_TOPIC, msg_text);
		client->publish(msg);
	}

	void Telemetry::on_gameplay_event(const scs_telemetry_gameplay_event_t* const event) {
		char sbuffer[64];
		snprintf(sbuffer, 64, "Recieved event with ID: %s", event->id);
		log(sbuffer);
	}

	void Telemetry::on_gear_changed(const scs_value_t* const value)
	{
		char sbuffer[32];
		snprintf(sbuffer, 32, "Gear changed: %d", value->value_s32.value);
		log(sbuffer);
	};
}