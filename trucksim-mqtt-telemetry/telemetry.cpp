#include "pch.h"

#include <stdio.h>

#include <nlohmann/json.hpp>

#include <mqtt/client.h>
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry_event.h"

#include "telemetry.h"
#include "logger/logger.h"
#include "topics.h"

using json = nlohmann::json;

namespace trucksim_mqtt {
	Telemetry::Telemetry(mqtt::client* client, Logger* logger) : client(client), logger(logger) {}

	void Telemetry::publish(const json* const data, const char* topic) const {
		auto mqtt_msg = mqtt::make_message(topic, data->dump());
		client->publish(mqtt_msg);
	}

	void Telemetry::version_check(const scs_telemetry_init_params_v101_t* const version_params) const
	{
		logger->info("Checking game version...");
		// Check application version. Note that this example uses fairly basic channels which are likely to be supported
		// by any future SCS trucking game however more advanced application might want to at least warn the user if there
		// is game or version they do not support.
		if (strcmp(version_params->common.game_id, SCS_GAME_ID_EUT2) == 0) {
			logger->info("Detected: ETS2!");
			// Below the minimum version there might be some missing features (only minor change) or
			// incompatible values (major change).
			const scs_u32_t MINIMAL_VERSION = SCS_TELEMETRY_EUT2_GAME_VERSION_1_00;
			if (version_params->common.game_version < MINIMAL_VERSION) {
				logger->warning("Too old version of the game, some features might behave incorrectly.");
			}

			// Future versions are fine as long the major version is not changed.
			const scs_u32_t IMPLEMENTED_VERSION = SCS_TELEMETRY_EUT2_GAME_VERSION_CURRENT;
			if (SCS_GET_MAJOR_VERSION(version_params->common.game_version) > SCS_GET_MAJOR_VERSION(IMPLEMENTED_VERSION)) {
				logger->warning("Too new major version of the game, some features might behave incorrectly");
			}
		}
		else if (strcmp(version_params->common.game_id, SCS_GAME_ID_ATS) == 0) {
			logger->info("Detected: ATS!");
			// Below the minimum version there might be some missing features (only minor change) or
			// incompatible values (major change).
			const scs_u32_t MINIMAL_VERSION = SCS_TELEMETRY_ATS_GAME_VERSION_1_00;
			if (version_params->common.game_version < MINIMAL_VERSION) {
				logger->warning("Too old version of the game, some features might behave incorrectly");
			}

			// Future versions are fine as long the major version is not changed.
			const scs_u32_t IMPLEMENTED_VERSION = SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT;
			if (SCS_GET_MAJOR_VERSION(version_params->common.game_version) > SCS_GET_MAJOR_VERSION(IMPLEMENTED_VERSION)) {
				logger->warning("Too new major version of the game, some features might behave incorrectly");
			}
		}
		else {
			logger->warning("Unsupported game, some features or values might behave incorrectly");
		}
	}

	void Telemetry::on_frame_start(const scs_event_t event, const scs_telemetry_frame_start_t* const event_info) {
		// noop
	}

	void Telemetry::on_frame_end(const scs_event_t event) {
		// noop
	}

	void Telemetry::on_pause(const scs_event_t event) {
		paused = (event == SCS_TELEMETRY_EVENT_paused);

		if (paused) logger->info("Telemetry paused...");
		else logger->info("Telemetry resumed...");
	}

	void Telemetry::on_gameplay_event(const scs_event_t event, const scs_telemetry_gameplay_event_t* const event_info) {
		json data;
		data["event_id"] = event_info->id;
		publish(&data, kEventsGameplayTopic);
	}

	void Telemetry::on_configuration_event(const scs_event_t event, const scs_telemetry_configuration_t* const event_info) {
		json data;
		data["event_id"] = event_info->id;
		publish(&data, kEventConfigTopic);
	}

	void Telemetry::on_gear_changed(const scs_value_t* const value)
	{
		json data;
		data["value"] = value->value_s32.value;
		publish(&data, kTruckEngineGearTopic);
	};

	void Telemetry::on_rpm_changed(const scs_value_t* const value) {
		json data;
		data["value"] = value->value_float.value;
		publish(&data, kTruckEngineRpmTopic);
	}
}