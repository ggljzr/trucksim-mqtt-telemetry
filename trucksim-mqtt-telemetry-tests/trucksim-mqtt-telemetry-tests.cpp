#include "pch.h"
#include "CppUnitTest.h"

#include "../trucksim-mqtt-telemetry/sdk/scssdk.h"
#include "../trucksim-mqtt-telemetry/sdk/common/scssdk_telemetry_truck_common_channels.h"
#include "../trucksim-mqtt-telemetry/client.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace trucksimmqtttelemetrytests
{
	TEST_CLASS(trucksimmqtttelemetrytests)
	{
	public:
		
		TEST_METHOD(TestChannelToTopic)
		{
			scs_string_t channel_name = SCS_TELEMETRY_TRUCK_CHANNEL_navigation_distance;
			auto topic = trucksim_mqtt::channel_to_topic(channel_name);
			Assert::AreEqual("trucksim/channel/truck/navigation/distance", topic.c_str());
		}
	};
}
