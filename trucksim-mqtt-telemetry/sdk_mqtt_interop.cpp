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

	void attrs_to_json(json& json, const scs_named_value_t* const attrs)
	{
		for (const scs_named_value_t* current = attrs; current->name; ++current) {
			switch (current->value.type) {
				case SCS_VALUE_TYPE_INVALID: {
					json[current->name] = nullptr;
					break;
				}
				case SCS_VALUE_TYPE_bool: {
					json[current->name] = current->value.value_bool.value;
					break;
				}
				case SCS_VALUE_TYPE_s32: {
					json[current->name] = current->value.value_s32.value;
					break;
				}
				case SCS_VALUE_TYPE_u32: {
					json[current->name] = current->value.value_u32.value;
					break;
				}
				case SCS_VALUE_TYPE_s64: {
					json[current->name] = current->value.value_s64.value;
					break;
				}
				case SCS_VALUE_TYPE_u64: {
					json[current->name] = current->value.value_u64.value;
					break;
				}
				case SCS_VALUE_TYPE_float: {
					json[current->name] = current->value.value_float.value;
					break;
				}
				case SCS_VALUE_TYPE_double: {
					json[current->name] = current->value.value_double.value;
					break;
				}
				case SCS_VALUE_TYPE_fvector: {
					auto vec = current->value.value_fvector;
					json[current->name]["x"] = vec.x;
					json[current->name]["y"] = vec.y;
					json[current->name]["z"] = vec.z;
					break;
				}
				case SCS_VALUE_TYPE_dvector: {
					auto vec = current->value.value_dvector;
					json[current->name]["x"] = vec.x;
					json[current->name]["y"] = vec.y;
					json[current->name]["z"] = vec.z;
					break;
				}
				case SCS_VALUE_TYPE_euler: {
					auto euler = current->value.value_euler;
					json[current->name]["heading"] = euler.heading * 360.0f;
					json[current->name]["pitch"] = euler.pitch * 360.0f;
					json[current->name]["roll"] = euler.roll * 360.0f;
					break;
				}
				case SCS_VALUE_TYPE_fplacement: {
					auto pos = current->value.value_fplacement.position;
					auto ori = current->value.value_fplacement.orientation;

					json[current->name]["position"]["x"] = pos.x;
					json[current->name]["position"]["y"] = pos.y;
					json[current->name]["position"]["z"] = pos.z;

					json[current->name]["orientation"]["heading"] = ori.heading * 360.0f;
					json[current->name]["orientation"]["pitch"] = ori.pitch * 360.0f;
					json[current->name]["orientation"]["roll"] = ori.roll * 360.0f;
					break;
				}
				case SCS_VALUE_TYPE_dplacement: {
					auto pos = current->value.value_dplacement.position;
					auto ori = current->value.value_dplacement.orientation;

					json[current->name]["position"]["x"] = pos.x;
					json[current->name]["position"]["y"] = pos.y;
					json[current->name]["position"]["z"] = pos.z;

					json[current->name]["orientation"]["heading"] = ori.heading * 360.0f;
					json[current->name]["orientation"]["pitch"] = ori.pitch * 360.0f;
					json[current->name]["orientation"]["roll"] = ori.roll * 360.0f;
					break;
				}
				case SCS_VALUE_TYPE_string: {
					json[current->name] = current->value.value_string.value;
					break;
				}
				default: {
					json[current->name] = "unknown";
					break;
				}
			}
		}
	}
}