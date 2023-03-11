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

	// Channel topics

	// Truck channels

	constexpr auto kTruckWorldPlacementTopic{ "trucksim/channel/truck/world_placement" }; // implemented, not registered
	constexpr auto kTruckSpeedTopic{ "trucksim/channel/truck/speed" };
	constexpr auto kTruckCruiseControlTopic{ "trucksim/channel/truck/cruise_control" }; // not implemented yet

	constexpr auto kTruckEngineRpmTopic{ "trucksim/channel/truck/engine/rpm" };
	constexpr auto kTruckEngineGearTopic{ "trucksim/channel/truck/engine/gear" };

	constexpr auto kTruckFuelAmountTopic{ "trucksim/channel/truck/fuel/amount" }; // not implemented yet
	constexpr auto kTruckFuelRangeTopic{ "trucksim/channel/truck/fuel/range" }; // not implemented yet
	constexpr auto kTruckFuelWarningTopic{ "trucksim/channel/truck/fuel/warning" }; // not implemented yet
}

