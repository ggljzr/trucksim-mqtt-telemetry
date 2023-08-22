#pragma once

#include <chrono>

#include <nlohmann/json.hpp>

#include <mqtt/async_client.h>
#include <mqtt/types.h>

#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry_event.h"

#include "logger/logger.h"

using json = nlohmann::json;

namespace trucksim_mqtt {
	/// <summary>
	/// Class responsible for publishing data from telemetry callbacks.
	/// </summary>
	class Telemetry {
	private:
		mqtt::async_client* client;
		Logger* logger;

		std::chrono::steady_clock::time_point last_world_placement_update{};
		std::chrono::steady_clock::time_point last_navigation_distance_update{};

		bool paused{ false };
	public:
		Telemetry(mqtt::async_client* client, Logger* logger);

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
		/// Basic publish method.
		/// </summary>
		void publish(mqtt::binary_ref payload, const char* topic) const;

		/// <summary>
		/// Published given JSON data to given topic. This method
		/// is safe to call even when MQTT client is not connected (no operation will be performed).
		/// </summary>
		void publish(const json* const data, const char* topic) const;

		/// <summary>
		/// Publishes single value in {"value": value} JSON. Note that topic really
		/// is a topic (e. g. "trucksim/channels/truck/engine/rpm") not a SCS SDK channel name 
		/// (e. g. "truck.engine.rpm"). Translation from channel names to topics is done in handlers.cpp.
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
		void on_config_event(const scs_event_t event, const scs_telemetry_configuration_t* const event_info) const;

		/// <summary>
		/// Method for publishing information about world placement.
		/// This method implements basic rate limiting: publishing the
		/// updates only if at least kWorldPlacementUpdatePeriod milliseconds passed
		/// since the last published world placement update.
		/// </summary>
		void on_world_placement(const scs_value_dplacement_t* const dplacement_val);

		/// <summary>
		/// Method for publishing information about navigation distance.
		/// This is also rate limited, see on_world_placement.
		/// </summary>
		/// <param name="distance"></param>
		void on_navigation_distance(const scs_value_float_t* const distance);

		/// <summary>
		/// Method called when the plugin is deinitialized.
		/// </summary>
		void on_goodbye();
	};
}