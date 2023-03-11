#pragma once

#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk_telemetry_event.h"

#include "telemetry.h"

namespace trucksim_mqtt {
	namespace handlers {

		#pragma region EventHandlers
		SCSAPI_VOID on_frame_start(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_frame_end(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_config_event(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_gameplay_event(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_pause(const scs_event_t event, const void* const event_info, const scs_context_t context);
		#pragma endregion

		#pragma region ChannelHandlers
		
		// For some reason this callback is called
		// even when the value does not change, so it is unregistered for now
		// (needs some caching implemented).
		SCSAPI_VOID on_world_placement(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context
		);

		SCSAPI_VOID on_speed_changed(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context
		);

		SCSAPI_VOID on_rpm_changed(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context
		);

		SCSAPI_VOID on_gear_changed(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context);

		SCSAPI_VOID on_fuel_amount(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context);

		SCSAPI_VOID on_fuel_range(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context);

		SCSAPI_VOID on_fuel_warning(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context);
		#pragma endregion

		/// <summary>
		/// Function that registers all telemetry handlers with SDK API. Note that this function takes Telemetry pointer
		/// as argument, ensuring that each handler will be correctly registered with this pointer as context.
		/// </summary>
		SCSAPI_RESULT register_handlers(const scs_telemetry_init_params_v101_t* const version_params, Telemetry* telemetry);
	}
}