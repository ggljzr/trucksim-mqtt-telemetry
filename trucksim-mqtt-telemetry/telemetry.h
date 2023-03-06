#pragma once

#include <mqtt/client.h>
#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"

namespace trucksim_mqtt {
	class Telemetry {
	private:
		mqtt::client* client;

	public:
		Telemetry(mqtt::client* client);
		SCSAPI_VOID on_gear_changed(const scs_value_t* const value);
	};
}