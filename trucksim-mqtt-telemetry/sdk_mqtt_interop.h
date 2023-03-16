/// Utilities for mapping SCS SDK API to MQTT API.
#pragma once

#include <string>

#include "sdk/scssdk.h"

namespace trucksim_mqtt {
	/// <summary>
	/// Translates the name from SCS SDK to MQTT topic string (with root topic prefix).
	/// Basically replaces "." with "/" and adds given topic prefix (like "trucksim/channel/").
	/// </summary>
	std::string sdkname_to_topic(const scs_string_t name, const char* topic_prefix);

	/// <summary>
	/// Translator for channel names:
	/// E. g. "truck.navigation.time" -> "trucksim/channel/truck/navigation/time"
	/// </summary>
	/// <returns></returns>
	std::string channel_to_topic(const scs_string_t name);

	/// <summary>
	/// Translator for config event names:
	/// E. g. "trailer.1" -> "trucksim/event/config/trailer/1"
	/// </summary>
	std::string config_to_topic(const scs_string_t name);

	/// <summary>
	/// Translator for gameplay event names:
	/// E. g. "player.fined" -> "trucksim/event/gameplay/player/fined"
	/// </summary>
	std::string gameplay_to_topic(const scs_string_t name);
}