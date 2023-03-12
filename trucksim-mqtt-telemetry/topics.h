/// Topics for MQTT API.
#pragma once

namespace trucksim_mqtt {
	/// <summary>
	/// Root plugin topic.
	/// </summary>
	constexpr auto kRootTopic{ "trucksim" };

	/// <summary>
	/// Topic used to publish plugin logs.
	/// </summary>
	constexpr auto kLogTopic{ "trucksim/log" };

	// Event topics

	constexpr auto kEventsGameplayTopic{ "trucksim/event/gameplay" };
	constexpr auto kEventConfigTopic{ "trucksim/event/config" };

	// Channel topic prefix, used to mapping SCS SDK constants to topics
	// note the trailing slash
	constexpr auto kChannelTopicPrefix{ "trucksim/channel/" };

	// Truck channels

	constexpr auto kTruckWorldPlacementTopic{ "trucksim/channel/truck/world_placement" }; // registered, time limited

	// Other currently implemented topics with universal handlers
	// "trucksim/channel/truck/speed" -- implemented, not registered
	// "trucksim/channel/truck/cruise_control"
	// "trucksim/channel/truck/engine/rpm" -- implemented, not registered
	// "trucksim/channel/truck/engine/gear"
	// "trucksim/channel/truck/fuel/amount"
	// "trucksim/channel/truck/fuel/range"
	// "trucksim/channel/truck/fuel/warning"
	// "trucksim/channel/truck/navigation/distance"
	// "trucksim/channel/truck/navigation/time"
	// "trucksim/channel/truck/navigation/speed/limit"

}

