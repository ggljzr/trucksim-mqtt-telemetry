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

	// Note that sometimes the event topics will publish null message
	// e. g. when the game menu is loaded, I guess to indicate clearing the configuration.

	// Gameplay event topic prefix, used to config events to topics.
	// note the trailing slash
	constexpr auto kEventsGameplayTopicPrefix{ "trucksim/event/gameplay/" };

	// Config event topic prefix, used to config events to topics.
	// note the trailing slash
	constexpr auto kEventConfigTopicPrefix{ "trucksim/event/config/" };

	// Config event topics

	// "trucksim/event/config/substances"
	// "trucksim/event/config/job"
	// "trucksim/event/config/controls"
	// "trucksim/event/config/hshifter"
	// "trucksim/event/config/truck"
	// "trucksim/event/config/trailer"
	// "trucksim/event/config/trailer/0"
	// "trucksim/event/config/trailer/1"
	// "trucksim/event/config/trailer/2"
	// "trucksim/event/config/trailer/3"
	// "trucksim/event/config/trailer/4"
	// "trucksim/event/config/trailer/5"
	// "trucksim/event/config/trailer/6"
	// "trucksim/event/config/trailer/7"
	// "trucksim/event/config/trailer/8"
	// "trucksim/event/config/trailer/9"

	// Channel topic prefix, used to map SCS SDK constants to topics
	// note the trailing slash
	constexpr auto kChannelTopicPrefix{ "trucksim/channel/" };

	// Game time channel
	// "trucksim/channel/game/time"

	// Truck channels

	constexpr auto kTruckWorldPlacementTopic{ "trucksim/channel/truck/world/placement" }; // registered, time limited

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

