#include "../pch.h"

#include <nlohmann/json.hpp>
#include <mqtt/client.h>

#include "logger.h"
#include "mqtt_logger.h"

using json = nlohmann::json;

namespace trucksim_mqtt {
	MqttLogger::MqttLogger(mqtt::client* client) : client(client) {};

	void MqttLogger::log(const char* msg, LogLevel level) {
		if (client->is_connected())
			return;

		json j;
		j["msg"] = msg;
		j["level"] = level;

		auto mqtt_msg = mqtt::make_message(kLogTopic, j.dump());

		try {
			client->publish(mqtt_msg);
		}
		catch(mqtt::timeout_error) {}
	}
}