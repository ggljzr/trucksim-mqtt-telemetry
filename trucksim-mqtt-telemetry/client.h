/// MQTT client utils.

#pragma once

#include <mqtt/client.h>

#include "sdk/scssdk_telemetry.h"

namespace trucksim_mqtt {
	/// <summary>
	/// Function that connects MQTT client to the broker.
	/// </summary>
	/// <returns>SCS_RESULT_ok if client successfully connects, SCS_RESULT_generic_error otherwise.</returns>
	SCSAPI_RESULT connect_client(mqtt::async_client* client, scs_log_t game_log);
}