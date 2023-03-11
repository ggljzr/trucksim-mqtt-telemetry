#include "pch.h"

#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk_telemetry_event.h"

#include "telemetry.h"

#include "handlers.h"

namespace trucksim_mqtt {
	namespace handlers {
		SCSAPI_VOID frame_start(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const struct scs_telemetry_frame_start_t* const info = static_cast<const scs_telemetry_frame_start_t*>(event_info);
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_frame_start(event, info);
		}

		SCSAPI_VOID frame_end(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_frame_end(event);
		}

		SCSAPI_VOID pause(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			Telemetry* telemetry = static_cast<Telemetry*>(context);
			telemetry->on_pause(event);
		}

		SCSAPI_VOID configuration(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const struct scs_telemetry_configuration_t* const info = static_cast<const scs_telemetry_configuration_t*>(event_info);
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_configuration_event(event, info);
		}

		SCSAPI_VOID gameplay_event(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const struct scs_telemetry_gameplay_event_t* const info = static_cast<const scs_telemetry_gameplay_event_t*>(event_info);
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_gameplay_event(event, info);
		}

		SCSAPI_VOID on_gear_changed(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
		{
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->publish_val(value->value_s32.value, kTruckEngineGearTopic);
		}

		SCSAPI_VOID on_rpm_changed(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context) {
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->publish_val(value->value_float.value, kTruckEngineRpmTopic);
		}

		SCSAPI_RESULT register_handlers(const scs_telemetry_init_params_v101_t* const version_params, const scs_context_t context)
		{
			const bool events_registered =
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, frame_start, NULL) == SCS_RESULT_ok) &&
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_end, frame_end, NULL) == SCS_RESULT_ok) &&
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, pause, NULL) == SCS_RESULT_ok) &&
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_started, pause, NULL) == SCS_RESULT_ok)
				;
			if (!events_registered) {
				version_params->common.log(SCS_LOG_TYPE_error, "Unable to register event callbacks");
				return SCS_RESULT_generic_error;
			}

			// Register for the configuration info. As this example only prints the retrieved
			// data, it can operate even if that fails.
			version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, configuration, NULL);

			// Register for gameplay events.
			version_params->register_for_event(SCS_TELEMETRY_EVENT_gameplay, gameplay_event, NULL);

			// Register for channels. The channel might be missing if the game does not support
			// it (SCS_RESULT_not_found) or if does not support the requested type
			// (SCS_RESULT_unsupported_type). For purpose of this example we ignore the failues
			// so the unsupported channels will remain at theirs default value.
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_gear, SCS_U32_NIL, SCS_VALUE_TYPE_s32, SCS_TELEMETRY_CHANNEL_FLAG_none, on_gear_changed, context);
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_rpm, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_rpm_changed, context);
			return SCS_RESULT_ok;
		}
	}
}