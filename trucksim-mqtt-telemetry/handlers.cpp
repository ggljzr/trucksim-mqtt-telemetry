#include "pch.h"

#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk_telemetry_event.h"

#include "telemetry.h"
#include "sdk_mqtt_interop.h"

#include "handlers.h"

namespace trucksim_mqtt {
	namespace handlers {

		#pragma region EventHandlers 
		SCSAPI_VOID on_frame_start(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const struct scs_telemetry_frame_start_t* const info = static_cast<const scs_telemetry_frame_start_t*>(event_info);
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_frame_start(event, info);
		}

		SCSAPI_VOID on_frame_end(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_frame_end(event);
		}

		SCSAPI_VOID on_pause(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			Telemetry* telemetry = static_cast<Telemetry*>(context);
			telemetry->on_pause(event);
		}

		SCSAPI_VOID on_config_event(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const struct scs_telemetry_configuration_t* const info = static_cast<const scs_telemetry_configuration_t*>(event_info);
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_config_event(event, info);
		}

		SCSAPI_VOID on_gameplay_event(const scs_event_t event, const void* const event_info, const scs_context_t context)
		{
			const struct scs_telemetry_gameplay_event_t* const info = static_cast<const scs_telemetry_gameplay_event_t*>(event_info);
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->on_gameplay_event(event, info);
		}
		#pragma endregion

		#pragma region ChannelHandlers
		SCSAPI_VOID on_world_placement(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
		{
			Telemetry* telemetry = static_cast<Telemetry*>(context);
			telemetry->on_world_placement(&value->value_dplacement);
		}

		SCSAPI_VOID on_navigation_distance(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
		{
			Telemetry* telemetry = static_cast<Telemetry*>(context);
			telemetry->on_navigation_distance(&value->value_float);
		}

		SCSAPI_VOID on_float_val(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
		{
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->publish_val(value->value_float.value, channel_to_topic(name).c_str());
		}

		SCSAPI_VOID on_u32_val(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
		{
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->publish_val(value->value_u32.value, channel_to_topic(name).c_str());
		}

		SCSAPI_VOID on_s32_val(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
		{
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->publish_val(value->value_s32.value, channel_to_topic(name).c_str());
		}

		SCSAPI_VOID on_bool_val(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context) {
			const Telemetry* telemetry = static_cast<const Telemetry*>(context);
			telemetry->publish_val(value->value_bool.value, channel_to_topic(name).c_str());
		}
		#pragma endregion

		SCSAPI_RESULT register_handlers(const scs_telemetry_init_params_v101_t* const version_params, Telemetry* telemetry)
		{
			const bool events_registered =
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, on_frame_start, telemetry) == SCS_RESULT_ok) &&
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_end, on_frame_end, telemetry) == SCS_RESULT_ok) &&
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, on_pause, telemetry) == SCS_RESULT_ok) &&
				(version_params->register_for_event(SCS_TELEMETRY_EVENT_started, on_pause, telemetry) == SCS_RESULT_ok)
				;
			if (!events_registered) {
				version_params->common.log(SCS_LOG_TYPE_error, "Unable to register event callbacks");
				return SCS_RESULT_generic_error;
			}

			// Register for the configuration info. As this example only prints the retrieved
			// data, it can operate even if that fails.
			version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, on_config_event, telemetry);

			// Register for gameplay events.
			version_params->register_for_event(SCS_TELEMETRY_EVENT_gameplay, on_gameplay_event, telemetry);

			// Register for channels. The channel might be missing if the game does not support
			// it (SCS_RESULT_not_found) or if does not support the requested type
			// (SCS_RESULT_unsupported_type). For purpose of this example we ignore the failues
			// so the unsupported channels will remain at theirs default value.

			// Game time
			version_params->register_for_channel(SCS_TELEMETRY_CHANNEL_game_time, SCS_U32_NIL, SCS_VALUE_TYPE_u32, SCS_TELEMETRY_CHANNEL_FLAG_none, on_u32_val, telemetry);

			// Next rest stop
			version_params->register_for_channel(SCS_TELEMETRY_CHANNEL_next_rest_stop, SCS_U32_NIL, SCS_VALUE_TYPE_s32, SCS_TELEMETRY_CHANNEL_FLAG_none, on_s32_val, telemetry);

			// Truck channels
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_world_placement, SCS_U32_NIL, SCS_VALUE_TYPE_dplacement, SCS_TELEMETRY_CHANNEL_FLAG_none, on_world_placement, telemetry);
			//version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_speed, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_bool_val, telemetry);
	
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_cruise_control, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_float_val, telemetry);

			//version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_rpm, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_float_val, telemetry);
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_gear, SCS_U32_NIL, SCS_VALUE_TYPE_s32, SCS_TELEMETRY_CHANNEL_FLAG_none, on_s32_val, telemetry);
		
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_float_val, telemetry);
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel_range, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_float_val, telemetry);
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel_warning, SCS_U32_NIL, SCS_VALUE_TYPE_bool, SCS_TELEMETRY_CHANNEL_FLAG_none, on_bool_val, telemetry);

			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_distance, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_navigation_distance, telemetry);
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_time, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_float_val, telemetry);
			version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_speed_limit, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_none, on_float_val, telemetry);

			return SCS_RESULT_ok;
		}
	}
}