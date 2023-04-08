// Precompiled header file for this project. Required by VS compiler.
#include "pch.h"

// Windows stuff.
#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#  define WINVER 0x0500
#  define _WIN32_WINNT 0x0500
#  include <windows.h>
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// MQTT Lib
#include <mqtt/async_client.h>

// SDK
#include "sdk/scssdk_telemetry.h"
#include "sdk/eurotrucks2/scssdk_eut2.h"
#include "sdk/eurotrucks2/scssdk_telemetry_eut2.h"
#include "sdk/amtrucks/scssdk_ats.h"
#include "sdk/amtrucks/scssdk_telemetry_ats.h"

// Local
#include "topics.h"
#include "config.h"
#include "client.h"
#include "logger/mqtt_logger.h"
#include "handlers.h"
#include "telemetry.h"

namespace trucksim_mqtt {
	constexpr auto dll_version = "v0.1.0";

	mqtt::async_client mqtt_client(kServerAddress, kClientId);
	MqttLogger logger(&mqtt_client);
	Telemetry telemetry(&mqtt_client, &logger);

	SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params)
	{
		// We currently support only one version.

		if (version != SCS_TELEMETRY_VERSION_1_01) {
			return SCS_RESULT_unsupported;
		}

		const scs_telemetry_init_params_v101_t* const version_params = static_cast<const scs_telemetry_init_params_v101_t*>(params);
		version_params->common.log(SCS_LOG_TYPE_message, "Initializing MQTT plugin...");

		// Connect to MQTT broker before anything else.
		// Pass the game log, just to log the error in case that
		// connecting fails.
		SCSAPI_RESULT result = connect_client(&mqtt_client, version_params->common.log);
		if (result != SCS_RESULT_ok) {
			return result;
		}

		// We are connected, do version check
		telemetry.version_check(version_params);

		SCSAPI_RESULT handlers_registered = handlers::register_handlers(version_params, &telemetry);
		if (result != SCS_RESULT_ok) {
			return result;
		}

		std::string ver_message("trucksim-mqtt-telemetry DLL version: ");
		ver_message.append(dll_version);
		logger.info(ver_message.c_str());
		logger.info("Handlers successfully registered.");

		return SCS_RESULT_ok;
	}

	/// <summary>
	/// Telemetry API deinitialization function. See scssdk_telemetry.h.
	/// </summary>
	SCSAPI_VOID scs_telemetry_shutdown(void)
	{
		telemetry.on_goodbye();
		auto token = mqtt_client.disconnect();
		token->wait_for(3000L);
	}

#ifdef _WIN32
	BOOL APIENTRY DllMain(
		HMODULE module,
		DWORD  reason_for_call,
		LPVOID reseved
	)
	{
		return TRUE;
	}
#endif

#ifdef __linux__
	void __attribute__((destructor)) unload(void)
	{
		finish_log();
	}
#endif

}