#pragma once

#include <mqtt/client.h>
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry_event.h"

namespace trucksim_mqtt {
	/// <summary>
	/// Class implementing callbacks for telemetry events and channels.
	/// </summary>
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
		/// Event handler for gameplay event (delivery finished, boarded ferry, etc.).
		/// </summary>
		void on_gameplay_event(const scs_telemetry_gameplay_event_t* const event);

		/// <summary>
		/// Event handler for configuration events.
		/// </summary>
		/// <param name="event"></param>
		void on_configuration_event(const scs_telemetry_configuration_t* const event);

		/// <summary>
		/// Event handler for changing gear.
		/// </summary>
		/// <param name="value">New gear value.</param>
		void on_gear_changed(const scs_value_t* const value);

		void on_rpm_changed(const scs_value_t* const value);
	};
}