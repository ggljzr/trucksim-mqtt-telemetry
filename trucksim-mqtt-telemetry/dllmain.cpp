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
#include <mqtt/client.h>

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
#include "telemetry.h"

#define UNUSED(x)

namespace trucksim_mqtt {
	mqtt::client mqtt_client(kServerAddress, kClientId, kPersistFile);
	MqttLogger logger(&mqtt_client);
	Telemetry telemetry(&mqtt_client, &logger);

	SCSAPI_VOID telemetry_frame_start(const scs_event_t event, const void* const event_info, const scs_context_t UNUSED(context))
	{
		const struct scs_telemetry_frame_start_t* const info = static_cast<const scs_telemetry_frame_start_t*>(event_info);
		telemetry.on_frame_start(event, info);
	}

	SCSAPI_VOID telemetry_frame_end(const scs_event_t event, const void* const UNUSED(event_info), const scs_context_t UNUSED(context))
	{
		telemetry.on_frame_end(event);
	}

	SCSAPI_VOID telemetry_pause(const scs_event_t event, const void* const UNUSED(event_info), const scs_context_t UNUSED(context))
	{
		telemetry.on_pause(event);
	}

	SCSAPI_VOID telemetry_configuration(const scs_event_t event, const void* const event_info, const scs_context_t UNUSED(context))
	{
		const struct scs_telemetry_configuration_t* const info = static_cast<const scs_telemetry_configuration_t*>(event_info);
		telemetry.on_configuration_event(event, info);
	}

	SCSAPI_VOID telemetry_gameplay_event(const scs_event_t event, const void* const event_info, const scs_context_t UNUSED(context))
	{
		const struct scs_telemetry_gameplay_event_t* const info = static_cast<const scs_telemetry_gameplay_event_t*>(event_info);
		telemetry.on_gameplay_event(event, info);
	}

	SCSAPI_VOID telemetry_on_gear_changed(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t UNUSED(context))
	{
		telemetry.on_gear_changed(value);
	}

	SCSAPI_VOID telemetry_on_rpm_changed(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t UNUSED(context)) {
		telemetry.on_rpm_changed(value);
	}

	/// <summary>
	///  Telemetry API initialization function. See scssdk_telemetry.h.
	/// </summary>
	SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params)
	{
		// We currently support only one version.

		if (version != SCS_TELEMETRY_VERSION_1_01) {
			return SCS_RESULT_unsupported;
		}

		const scs_telemetry_init_params_v101_t* const version_params = static_cast<const scs_telemetry_init_params_v101_t*>(params);
		version_params->common.log(SCS_LOG_TYPE_message, "Initializing MQTT plugin...");

		// Connect to MQTT broker
		// Pass the game log, just to log the error in case that
		// connecting fails.
		SCSAPI_RESULT result = connect_client(&mqtt_client, version_params->common.log);
		if (result != SCS_RESULT_ok) {
			return result;
		}

		// We are connected, do version check
		telemetry.version_check(version_params);

		// Register for events. Note that failure to register those basic events
		// likely indicates invalid usage of the api or some critical problem. As the
		// example requires all of them, we can not continue if the registration fails.
		const bool events_registered =
			(version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, telemetry_frame_start, NULL) == SCS_RESULT_ok) &&
			(version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_end, telemetry_frame_end, NULL) == SCS_RESULT_ok) &&
			(version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, telemetry_pause, NULL) == SCS_RESULT_ok) &&
			(version_params->register_for_event(SCS_TELEMETRY_EVENT_started, telemetry_pause, NULL) == SCS_RESULT_ok)
			;
		if (!events_registered) {

			// Registrations created by unsuccessfull initialization are
			// cleared automatically so we can simply exit.
			version_params->common.log(SCS_LOG_TYPE_error, "Unable to register event callbacks");
			return SCS_RESULT_generic_error;
		}

		// Register for the configuration info. As this example only prints the retrieved
		// data, it can operate even if that fails.
		version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, telemetry_configuration, NULL);

		// Register for gameplay events.
		version_params->register_for_event(SCS_TELEMETRY_EVENT_gameplay, telemetry_gameplay_event, NULL);

		// Register for channels. The channel might be missing if the game does not support
		// it (SCS_RESULT_not_found) or if does not support the requested type
		// (SCS_RESULT_unsupported_type). For purpose of this example we ignore the failues
		// so the unsupported channels will remain at theirs default value.
		version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_gear, SCS_U32_NIL, SCS_VALUE_TYPE_s32, SCS_TELEMETRY_CHANNEL_FLAG_none, telemetry_on_gear_changed, NULL);
		version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_rpm, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, telemetry_on_rpm_changed, NULL);


		return SCS_RESULT_ok;
	}

	/// <summary>
	/// Telemetry API deinitialization function. See scssdk_telemetry.h.
	/// </summary>
	SCSAPI_VOID scs_telemetry_shutdown(void)
	{
		logger.info("Goodbye...");
		mqtt_client.disconnect();
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