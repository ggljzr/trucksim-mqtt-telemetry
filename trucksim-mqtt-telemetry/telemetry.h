#pragma once

#include <nlohmann/json.hpp>

#include <mqtt/client.h>

#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry_event.h"

#include "logger/logger.h"
#include "topics.h"

using json = nlohmann::json;

namespace trucksim_mqtt {
	/// <summary>
	/// Class implementing callbacks for telemetry events and channels.
	/// </summary>
	class Telemetry {
	private:
		mqtt::client* client;
		Logger* logger;

		bool paused{ false };
	public:
		Telemetry(mqtt::client* client, Logger* logger);

		/// <summary>
		/// Runs the version check and logs the information based on game version.
		/// Note that this method is purely for logging, after the basic version check in scs_telemetry_init.
		/// </summary>
		void version_check(const scs_telemetry_init_params_v101_t* const version_params) const;

		/// <summary>
		/// Returns true if the game is currently paused. Tracked by on_pause event handler.
		/// </summary>
		bool is_paused() const { return paused; };

		/// <summary>
		/// Published given JSON data to given topic
		/// </summary>
		void publish(const json* const data, const char* topic) const;

		/// <summary>
		/// Publishes single value in {"value": value} JSON.
		/// </summary>
		template<typename T> 
		void publish_val(const T value, const char* topic) const;

		/// <summary>
		/// Currently not implemented. Empty callback.
		/// 
		/// TODO: maybe update timestamp internally, without MQTT message?
		/// </summary>
		void on_frame_start(const scs_event_t event, const scs_telemetry_frame_start_t* const event_info) const;

		/// <summary>
		/// Currently not implemented. Empty callback.
		/// </summary>
		void on_frame_end(const scs_event_t event) const;

		void on_pause(const scs_event_t event);

		void on_gameplay_event(const scs_event_t event, const scs_telemetry_gameplay_event_t* const event_info) const;
		void on_configuration_event(const scs_event_t event, const scs_telemetry_configuration_t* const event_info) const;
	};
}