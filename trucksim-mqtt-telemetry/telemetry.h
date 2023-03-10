#pragma once

#include <mqtt/client.h>

#include "sdk/scssdk_telemetry.h"
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

		/// <summary>
		/// Runs the version check and logs the information based on game version.
		/// Note that this method is purely for logging, after the basic version check in scs_telemetry_init.
		/// </summary>
		void version_check(const scs_telemetry_init_params_v101_t* const version_params) const;

		// Event handlers
		void on_gameplay_event(const scs_telemetry_gameplay_event_t* const event_info);
		void on_configuration_event(const scs_telemetry_configuration_t* const event_info);

		// Channel handlers
		void on_gear_changed(const scs_value_t* const value);
		void on_rpm_changed(const scs_value_t* const value);
	};
}