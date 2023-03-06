#include "pch.h"

#include <mqtt/client.h>
#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"

#include "telemetry.h"
#include "topics.h"

namespace trucksim_mqtt {
	Telemetry::Telemetry(mqtt::client* client) : client(client) {}

	SCSAPI_VOID Telemetry::on_gear_changed(const scs_value_t* const value)
	{
		char sbuffer[32];
		snprintf(sbuffer, 32, "Gear changed: %d", value->value_s32.value);

		auto msg = mqtt::make_message(LOG_TOPIC, sbuffer);
		client->publish(msg);
	};
}