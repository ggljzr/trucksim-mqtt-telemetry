/// Compile time configuration values.
#pragma once

namespace trucksim_mqtt {
	constexpr auto SERVER_ADDRESS{ "tcp://127.0.0.1:1883" };
	constexpr auto CLIENT_ID{ "trucksim-publisher" };
	constexpr auto PERSIST_FILE{ "C:/Users/gogol/source/persist" };
}