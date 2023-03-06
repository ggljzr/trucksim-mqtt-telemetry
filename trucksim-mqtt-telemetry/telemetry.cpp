#include "pch.h"

#include <mqtt/client.h>
#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"

#include "telemetry.h"
#include "topics.h"

namespace trucksim_mqtt {
	Telemetry::Telemetry(mqtt::client* client) : client(client) {}

	void Telemetry::log(const char* msg_text) {
		auto msg = mqtt::make_message(LOG_TOPIC, msg_text);
		client->publish(msg);
	}

	SCSAPI_VOID Telemetry::on_gear_changed(const scs_value_t* const value)
	{
		char sbuffer[32];
		snprintf(sbuffer, 32, "Gear changed: %d", value->value_s32.value);
		log(sbuffer);
	};
}