#pragma once

#include <mqtt/async_client.h>

#include "../topics.h"
#include "logger.h"

namespace trucksim_mqtt {
	/// <summary>
	/// Class for logging messages via MQTT.
	/// </summary>
	class MqttLogger: public Logger {
	private:
		mqtt::async_client* client;
	public:
		MqttLogger(mqtt::async_client* client);

		/// <summary>
		/// Publishes message into plugin logging topic. This method
		/// is safe to call even when MQTT client is not connected (no operation will be performed).
		/// </summary>
		virtual void log(const char* msg, LogLevel level) override;
	};
}