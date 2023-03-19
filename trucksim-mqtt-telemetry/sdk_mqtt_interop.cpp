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

			// replace dots in attribute name with underscores
			// to prevent confusion in javascript,
			// where dots would conflict with object notation
			// TODO: it would be nice if this function would actually
			// load names with dots recursively into JSON subkeys
			auto name = std::string(current->name);
			std::replace(name.begin(), name.end(), '.', '_');

			switch (current->value.type) {
				case SCS_VALUE_TYPE_INVALID: {
					json[name] = nullptr;
					break;
				}
				case SCS_VALUE_TYPE_bool: {
					json[name] = current->value.value_bool.value;
					break;
				}
				case SCS_VALUE_TYPE_s32: {
					json[name] = current->value.value_s32.value;
					break;
				}
				case SCS_VALUE_TYPE_u32: {
					json[name] = current->value.value_u32.value;
					break;
				}
				case SCS_VALUE_TYPE_s64: {
					json[name] = current->value.value_s64.value;
					break;
				}
				case SCS_VALUE_TYPE_u64: {
					json[name] = current->value.value_u64.value;
					break;
				}
				case SCS_VALUE_TYPE_float: {
					json[name] = current->value.value_float.value;
					break;
				}
				case SCS_VALUE_TYPE_double: {
					json[name] = current->value.value_double.value;
					break;
				}
				case SCS_VALUE_TYPE_fvector: {
					auto vec = current->value.value_fvector;
					json[name]["x"] = vec.x;
					json[name]["y"] = vec.y;
					json[name]["z"] = vec.z;
					break;
				}
				case SCS_VALUE_TYPE_dvector: {
					auto vec = current->value.value_dvector;
					json[name]["x"] = vec.x;
					json[name]["y"] = vec.y;
					json[name]["z"] = vec.z;
					break;
				}
				case SCS_VALUE_TYPE_euler: {
					auto euler = current->value.value_euler;
					json[name]["heading"] = euler.heading * 360.0f;
					json[name]["pitch"] = euler.pitch * 360.0f;
					json[name]["roll"] = euler.roll * 360.0f;
					break;
				}
				case SCS_VALUE_TYPE_fplacement: {
					auto pos = current->value.value_fplacement.position;
					auto ori = current->value.value_fplacement.orientation;

					json[name]["position"]["x"] = pos.x;
					json[name]["position"]["y"] = pos.y;
					json[name]["position"]["z"] = pos.z;

					json[name]["orientation"]["heading"] = ori.heading * 360.0f;
					json[name]["orientation"]["pitch"] = ori.pitch * 360.0f;
					json[name]["orientation"]["roll"] = ori.roll * 360.0f;
					break;
				}
				case SCS_VALUE_TYPE_dplacement: {
					auto pos = current->value.value_dplacement.position;
					auto ori = current->value.value_dplacement.orientation;

					json[name]["position"]["x"] = pos.x;
					json[name]["position"]["y"] = pos.y;
					json[name]["position"]["z"] = pos.z;

					json[name]["orientation"]["heading"] = ori.heading * 360.0f;
					json[name]["orientation"]["pitch"] = ori.pitch * 360.0f;
					json[name]["orientation"]["roll"] = ori.roll * 360.0f;
					break;
				}
				case SCS_VALUE_TYPE_string: {
					json[name] = current->value.value_string.value;
					break;
				}
				default: {
					json[name] = "unknown";
					break;
				}
			}
		}
	}
}