/// Compile time configuration values.
#pragma once

#include <chrono>

namespace trucksim_mqtt {
	constexpr auto kServerAddress{ "tcp://127.0.0.1:1883" };
	constexpr auto kClientId{ "trucksim-publisher" };
	constexpr std::chrono::milliseconds kWorldPlacementUpdatePeriod{ 100 };
	constexpr std::chrono::milliseconds kNavigationDistanceUpdatePeriod{ 1000 };
}