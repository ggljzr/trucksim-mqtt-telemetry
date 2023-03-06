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

		/// <summary>
		/// Method for sending log messages to LOG topic.
		/// </summary>
		void log(const char* msg_text);

		/// <summary>
		/// Event handler for changing gear.
		/// </summary>
		/// <param name="value">New gear value.</param>
		SCSAPI_VOID on_gear_changed(const scs_value_t* const value);
	};
}