#include "pch.h"

#include "sdk/scssdk.h"

#include "sdk_mqtt_interop.h"
#include "topics.h"

namespace trucksim_mqtt {
	std::string sdkname_to_topic(const scs_string_t name, const char* topic_prefix)
	{
		auto topic_string = std::string(name);
		std::replace(topic_string.begin(), topic_string.end(), '.', '/');
		topic_string.insert(0, topic_prefix);
		return topic_string;
	}

	std::string channel_to_topic(const scs_string_t name) {
		return sdkname_to_topic(name, kChannelTopicPrefix);
	}

	std::string config_to_topic(const scs_string_t name) {
		return sdkname_to_topic(name, kEventConfigTopicPrefix);
	}

	std::string gameplay_to_topic(const scs_string_t name) {
		return sdkname_to_topic(name, kEventsGameplayTopicPrefix);
	}
}