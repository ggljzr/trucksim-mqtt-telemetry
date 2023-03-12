/// MQTT client utils.

#pragma once

#include <string>
#include <stdlib.h>
#include <mqtt/client.h>

#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk.h"

namespace trucksim_mqtt {
	/// <summary>
	/// Function that connects MQTT client to the broker.
	/// </summary>
	/// <returns>SCS_RESULT_ok if client successfully connects, SCS_RESULT_generic_error otherwise.</returns>
	SCSAPI_RESULT connect_client(mqtt::client* client, scs_log_t game_log);

	/// <summary>
	/// Translates the channel name from SCS SDK to MQTT topic string (with root topic prefix).
	/// E. g. "truck.navigation.time" -> "trucksim/channel/truck/navigation/time"
	/// </summary>
	/// <param name="channel_name"></param>
	/// <returns></returns>
	std::string channel_to_topic(const scs_string_t channel_name);
}