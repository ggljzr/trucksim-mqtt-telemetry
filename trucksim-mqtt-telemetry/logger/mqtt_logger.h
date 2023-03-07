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

		virtual void log(const char* msg, LogLevel level) override;
	};
}