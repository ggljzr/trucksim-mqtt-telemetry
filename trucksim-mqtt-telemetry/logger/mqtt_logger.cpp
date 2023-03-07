#include "../pch.h"

#include <mqtt/client.h>

#include "logger.h"
#include "mqtt_logger.h"

namespace trucksim_mqtt {
	MqttLogger::MqttLogger(mqtt::client* client) : client(client) {};

	void MqttLogger::log(const char* msg, LogLevel level) {
		auto mqtt_msg = mqtt::make_message(kLogTopic, msg);
		client->publish(mqtt_msg);
	}
}