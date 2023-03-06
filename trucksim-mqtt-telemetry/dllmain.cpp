// Precompiled header file for this project. Required by VS compiler.
#include "pch.h"

// Windows stuff.
#ifdef _WIN32
#  define WINVER 0x0500
#  define _WIN32_WINNT 0x0500
#  include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

#define UNUSED(x)

namespace trucksim_mqtt {
	mqtt::client mqtt_client(SERVER_ADDRESS, CLIENT_ID, PERSIST_FILE);

	/// <summary>
	/// Tracking of paused state of the game.
	/// </summary>
	bool output_paused = true;

	/// <summary>
	/// Last timestamp we received.
	/// </summary>
	scs_timestamp_t last_timestamp = static_cast<scs_timestamp_t>(-1);

	/// <summary>
	/// Function writting message to the game internal log. Initialized in scs_telemetry_init.
	/// </summary>
	scs_log_t game_log = NULL;

	/// <summary>
	/// Function that connects MQTT client to the broker.
	/// </summary>
	/// <returns>SCS_RESULT_ok if client successfully connects, SCS_RESULT_generic_error otherwise.</returns>
	SCSAPI_RESULT connect_client() {
		mqtt::connect_options conn_opts;
		conn_opts.set_keep_alive_interval(20);
		conn_opts.set_clean_session(true);

		try {
			mqtt_client.connect(conn_opts);
		}
		catch (const mqtt::exception& exc) {

			if (game_log != NULL) {
				game_log(SCS_LOG_TYPE_message, "MQTT Connection error");
				game_log(SCS_LOG_TYPE_message, exc.what());
			}

			return SCS_RESULT_generic_error;
		}

		auto msg = mqtt::make_message(LOG_TOPIC, "Hello from ATS!");
		mqtt_client.publish(msg);

		return SCS_RESULT_ok;
	}

	// Handling of individual events.

	SCSAPI_VOID telemetry_frame_start(const scs_event_t UNUSED(event), const void* const event_info, const scs_context_t UNUSED(context))
	{
		const struct scs_telemetry_frame_start_t* const info = static_cast<const scs_telemetry_frame_start_t*>(event_info);

		// The following processing of the timestamps is done so the output
		// from this plugin has continuous time, it is not necessary otherwise.

		// When we just initialized itself, assume that the time started
		// just now.

		if (last_timestamp == static_cast<scs_timestamp_t>(-1)) {
			last_timestamp = info->paused_simulation_time;
		}

		// The timer might be sometimes restarted (e.g. after load) while
		// we want to provide continuous time on our output.

		if (info->flags & SCS_TELEMETRY_FRAME_START_FLAG_timer_restart) {
			last_timestamp = 0;
		}

		// Advance the timestamp by delta since last frame.
		last_timestamp = info->paused_simulation_time;
	}

	SCSAPI_VOID telemetry_frame_end(const scs_event_t UNUSED(event), const void* const UNUSED(event_info), const scs_context_t UNUSED(context))
	{
		if (output_paused) {
			return;
		}

		// The header.
	}

	SCSAPI_VOID telemetry_pause(const scs_event_t event, const void* const UNUSED(event_info), const scs_context_t UNUSED(context))
	{
		output_paused = (event == SCS_TELEMETRY_EVENT_paused);
	}

	SCSAPI_VOID telemetry_configuration(const scs_event_t event, const void* const event_info, const scs_context_t UNUSED(context))
	{
		// Here we just print the configuration info.

		const struct scs_telemetry_configuration_t* const info = static_cast<const scs_telemetry_configuration_t*>(event_info);
	}

	SCSAPI_VOID telemetry_gameplay_event(const scs_event_t event, const void* const event_info, const scs_context_t UNUSED(context))
	{
		// Here we just print the event info.

		const struct scs_telemetry_gameplay_event_t* const info = static_cast<const scs_telemetry_gameplay_event_t*>(event_info);
	}

	// Handling of individual channels.

	SCSAPI_VOID telemetry_store_orientation(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
	{
		assert(context);

		// This callback was registered with the SCS_TELEMETRY_CHANNEL_FLAG_no_value flag
		// so it is called even when the value is not available.

		if (!value) {
			return;
		}

		assert(value);
		assert(value->type == SCS_VALUE_TYPE_euler);
	}

	SCSAPI_VOID telemetry_on_gear_changed(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context) {
		char sbuffer[32];
		snprintf(sbuffer, 32, "Gear changed: %d", value->value_s32.value);

		if (game_log != NULL) {
			game_log(SCS_LOG_TYPE_message, sbuffer);
		}

		auto msg = mqtt::make_message(LOG_TOPIC, sbuffer);
		mqtt_client.publish(msg);
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

		// Check application version. Note that this example uses fairly basic channels which are likely to be supported
		// by any future SCS trucking game however more advanced application might want to at least warn the user if there
		// is game or version they do not support.

		if (strcmp(version_params->common.game_id, SCS_GAME_ID_EUT2) == 0) {

			// Below the minimum version there might be some missing features (only minor change) or
			// incompatible values (major change).

			const scs_u32_t MINIMAL_VERSION = SCS_TELEMETRY_EUT2_GAME_VERSION_1_00;
			if (version_params->common.game_version < MINIMAL_VERSION) {
				// log_line("WARNING: Too old version of the game, some features might behave incorrectly");
			}

			// Future versions are fine as long the major version is not changed.

			const scs_u32_t IMPLEMENTED_VERSION = SCS_TELEMETRY_EUT2_GAME_VERSION_CURRENT;
			if (SCS_GET_MAJOR_VERSION(version_params->common.game_version) > SCS_GET_MAJOR_VERSION(IMPLEMENTED_VERSION)) {
				// log_line("WARNING: Too new major version of the game, some features might behave incorrectly");
			}
		}
		else if (strcmp(version_params->common.game_id, SCS_GAME_ID_ATS) == 0) {

			// Below the minimum version there might be some missing features (only minor change) or
			// incompatible values (major change).

			const scs_u32_t MINIMAL_VERSION = SCS_TELEMETRY_ATS_GAME_VERSION_1_00;
			if (version_params->common.game_version < MINIMAL_VERSION) {
				// log_line("WARNING: Too old version of the game, some features might behave incorrectly");
			}

			// Future versions are fine as long the major version is not changed.

			const scs_u32_t IMPLEMENTED_VERSION = SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT;
			if (SCS_GET_MAJOR_VERSION(version_params->common.game_version) > SCS_GET_MAJOR_VERSION(IMPLEMENTED_VERSION)) {
				// log_line("WARNING: Too new major version of the game, some features might behave incorrectly");
			}
		}
		else {
			// log_line("WARNING: Unsupported game, some features or values might behave incorrectly");
		}

		// Remember the function we will use for logging.

		game_log = version_params->common.log;
		game_log(SCS_LOG_TYPE_message, "Initializing MQTT plugin...");

		// Connect to MQTT broker
		SCSAPI_RESULT result = connect_client();
		if (result != SCS_RESULT_ok) {
			return result;
		}


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

		// Set the structure with defaults.
		last_timestamp = static_cast<scs_timestamp_t>(-1);

		// Initially the game is paused.
		output_paused = true;
		return SCS_RESULT_ok;
	}

	/// <summary>
	/// Telemetry API deinitialization function. See scssdk_telemetry.h.
	/// </summary>
	SCSAPI_VOID scs_telemetry_shutdown(void)
	{
		// Any cleanup needed. The registrations will be removed automatically
		// so there is no need to do that manually.

		game_log = NULL;
		auto msg = mqtt::make_message(LOG_TOPIC, "Goodbye from ATS!");
		mqtt_client.publish(msg);
		mqtt_client.disconnect();
	}

	// Cleanup

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