/// Topics for MQTT API.
#pragma once

namespace trucksim_mqtt {
	/// <summary>
	/// Root plugin topic.
	/// </summary>
	constexpr auto ROOT_TOPIC{ "trucksim" };

	/// <summary>
	/// Topic used to publish plugin logs.
	/// </summary>
	constexpr auto LOG_TOPIC{ "trucksim/log" };
}

