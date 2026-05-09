#include "pch.h"

#include <string>
#include <stdlib.h>
#include <algorithm>

#include <mqtt/client.h>
#include "sdk/scssdk_telemetry.h"

#include "topics.h"
#include "client.h"


namespace trucksim_mqtt {
	SCSAPI_RESULT connect_client(mqtt::async_client* client, scs_log_t game_log, long timeout) {
		mqtt::connect_options conn_opts;
		conn_opts.set_keep_alive_interval(20);
		conn_opts.set_clean_session(true);

		if (game_log != NULL) {
			game_log(SCS_LOG_TYPE_message, "Connecting to MQTT server...");
			game_log(SCS_LOG_TYPE_message, client->get_server_uri().c_str());
		}

		try {
			auto token = client->connect(conn_opts);
			// Blocking wait for client to connect.
			const bool completed = token->wait_for(timeout);
			if (!completed || !client->is_connected()) {
				if (game_log != NULL) {
					game_log(SCS_LOG_TYPE_warning, "MQTT connection timeout or broker unreachable");
				}
				return SCS_RESULT_generic_error;
			}
		}
		catch (const mqtt::exception& exc) {

			if (game_log != NULL) {
				game_log(SCS_LOG_TYPE_warning, "MQTT Connection error");
				game_log(SCS_LOG_TYPE_warning, exc.what());
			}

			return SCS_RESULT_generic_error;
		}

		if (game_log != NULL) {
			game_log(SCS_LOG_TYPE_message, "Connection successful");
		}

		return SCS_RESULT_ok;
	}
}