#include "../pch.h"

#include <nlohmann/json.hpp>
#include <mqtt/client.h>

#include "logger.h"
#include "mqtt_logger.h"

using json = nlohmann::json;

namespace trucksim_mqtt {
	MqttLogger::MqttLogger(mqtt::client* client) : client(client) {};

	void MqttLogger::log(const char* msg, LogLevel level) {
		json j;
		j["msg"] = msg;
		j["level"] = level;

		auto mqtt_msg = mqtt::make_message(kLogTopic, j.dump());
		client->publish(mqtt_msg);
	}
}