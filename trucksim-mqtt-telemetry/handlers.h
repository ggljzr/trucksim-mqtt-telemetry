#pragma once

#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk_telemetry_event.h"

#include "telemetry.h"

namespace trucksim_mqtt {

	/// <summary>
	/// Handlers registered with SDK API, used to delegate calls to Telemetry methods. Telemetry
	/// instance is always passed via context argument.
	/// </summary>
	namespace handlers {

		#pragma region EventHandlers
		SCSAPI_VOID on_frame_start(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_frame_end(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_config_event(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_gameplay_event(const scs_event_t event, const void* const event_info, const scs_context_t context);
		SCSAPI_VOID on_pause(const scs_event_t event, const void* const event_info, const scs_context_t context);
		#pragma endregion

		#pragma region ChannelHandlers
		
		/// <summary>
		/// Special callback for world placement.
		/// </summary>
		SCSAPI_VOID on_world_placement(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context
		);

		/// <summary>
		/// Universal callback for float channels.
		/// </summary>
		/// <param name="name">used for topic resolution</param>
		/// <param name="context">pointer to Telemetry instance</param>
		SCSAPI_VOID on_float_val(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context
		);

		/// <summary>
		/// Universal callback fro s32 channels.
		/// </summary>
		/// <param name="name">used for topic resolution</param>
		/// <param name="context">pointer to Telemetry instance</param>
		SCSAPI_VOID on_s32_val(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context
		);

		/// <summary>
		/// Universal callback for bool channels.
		/// </summary>
		/// <param name="name">used for topic resolution</param>
		/// <param name="context">pointer to Telemetry instance</param>
		SCSAPI_VOID on_bool_val(
			const scs_string_t name,
			const scs_u32_t index,
			const scs_value_t* const value,
			const scs_context_t context
		);
		#pragma endregion

		/// <summary>
		/// Function that registers all telemetry handlers with SDK API. Note that this function takes Telemetry pointer
		/// as argument, ensuring that each handler will be correctly registered with this pointer as context.
		/// </summary>
		SCSAPI_RESULT register_handlers(const scs_telemetry_init_params_v101_t* const version_params, Telemetry* telemetry);
	}
}