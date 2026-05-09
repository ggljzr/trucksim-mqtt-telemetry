/// Compile time configuration values.
#pragma once

#include <chrono>
#include <string>

#include "sdk/scssdk_telemetry.h"

namespace trucksim_mqtt {
	constexpr auto kServerAddress{ "tcp://127.0.0.1:1883" };
	constexpr auto kClientId{ "trucksim-publisher" };
	constexpr std::chrono::milliseconds kWorldPlacementUpdatePeriod{ 100 };
	constexpr std::chrono::milliseconds kNavigationDistanceUpdatePeriod{ 1000 };

	struct Config {
		std::string server_address{ kServerAddress };
	};

	Config load_config(const std::string& path, scs_log_t game_log);
}