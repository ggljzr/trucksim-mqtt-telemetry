#pragma once

#include <mqtt/client.h>

#include "../topics.h"
#include "logger.h"

namespace trucksim_mqtt {
	/// <summary>
	/// Class for logging messages via MQTT.
	/// </summary>
	class MqttLogger: public Logger {
	private:
		mqtt::client* client;
	public:
		MqttLogger(mqtt::client* client);

		/// <summary>
		/// Publishes message into plugin logging topic. This method
		/// is safe to call even when MQTT client is not connected (no operation will be performed).
		/// </summary>
		virtual void log(const char* msg, LogLevel level) override;
	};
}