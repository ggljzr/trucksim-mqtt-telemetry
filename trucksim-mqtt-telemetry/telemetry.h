#pragma once

#include <mqtt/client.h>
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry_event.h"

#include "logger/logger.h"

namespace trucksim_mqtt {
	/// <summary>
	/// Class implementing callbacks for telemetry events and channels.
	/// </summary>
	class Telemetry {
	private:
		mqtt::client* client;
		Logger* logger;

	public:
		Telemetry(mqtt::client* client, Logger* logger);

		// Event handlers
		void on_gameplay_event(const scs_telemetry_gameplay_event_t* const event);
		void on_configuration_event(const scs_telemetry_configuration_t* const event);

		// Channel handlers
		void on_gear_changed(const scs_value_t* const value);
		void on_rpm_changed(const scs_value_t* const value);
	};
}