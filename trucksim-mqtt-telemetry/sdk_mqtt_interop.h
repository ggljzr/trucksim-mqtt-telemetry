/// Utilities for mapping SCS SDK API to MQTT API.
#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"

using json = nlohmann::json;

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

	/// <summary>
	/// Loads values from given event attributes struct into JSON.
	/// 
	/// I am deliberately not using arbitrary type conversions from nlohmann's JSON library
	/// because in that case this function would have to be in global namespace.
	/// 
	/// See https://github.com/nlohmann/json#basic-usage.
	/// 
	/// Implementation is basically copied from SCS SDK telemetry.cpp example.
	/// </summary>
	void attrs_to_json(json& json, const scs_named_value_t* const attrs);
}