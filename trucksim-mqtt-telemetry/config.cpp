#include "pch.h"

#include <fstream>

#include <nlohmann/json.hpp>

#include "config.h"

using json = nlohmann::json;

namespace trucksim_mqtt {
	Config load_config(const std::string& path, scs_log_t game_log) {
		Config config;

		std::ifstream input(path);
		if (!input.is_open()) {
			return config;
		}

		try {
			const auto parsed = json::parse(input);
			if (parsed.contains("server_address") && parsed["server_address"].is_string()) {
				config.server_address = parsed["server_address"].get<std::string>();
			}
		}
		catch (const std::exception& exc) {
			if (game_log != NULL) {
				game_log(SCS_LOG_TYPE_warning, "Unable to parse config file, using defaults");
				game_log(SCS_LOG_TYPE_warning, exc.what());
			}
		}

		return config;
	}
}
