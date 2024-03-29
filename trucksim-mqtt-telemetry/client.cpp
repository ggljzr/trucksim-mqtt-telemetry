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

		try {
			auto token = client->connect(conn_opts);
			// blocking wait for client to connect
			token->wait_for(timeout);
		}
		catch (const mqtt::exception& exc) {

			if (game_log != NULL) {
				game_log(SCS_LOG_TYPE_message, "MQTT Connection error");
				game_log(SCS_LOG_TYPE_message, exc.what());
			}

			return SCS_RESULT_generic_error;
		}

		return SCS_RESULT_ok;
	}
}