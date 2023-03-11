#pragma once

#include "sdk/scssdk.h"
#include "sdk/scssdk_value.h"
#include "sdk/scssdk_telemetry.h"
#include "sdk/scssdk_telemetry_event.h"

// Channel handlers

namespace trucksim_mqtt {
	SCSAPI_VOID telemetry_frame_start(const scs_event_t event, const void* const event_info, const scs_context_t context);
	SCSAPI_VOID telemetry_frame_end(const scs_event_t event, const void* const event_info, const scs_context_t context);
	SCSAPI_VOID telemetry_configuration(const scs_event_t event, const void* const event_info, const scs_context_t context);
	SCSAPI_VOID telemetry_gameplay_event(const scs_event_t event, const void* const event_info, const scs_context_t context);
	SCSAPI_VOID telemetry_pause(const scs_event_t event, const void* const event_info, const scs_context_t context);

	SCSAPI_VOID telemetry_on_rpm_changed(
		const scs_string_t name,
		const scs_u32_t index,
		const scs_value_t* const value,
		const scs_context_t context
	);

	SCSAPI_VOID telemetry_on_gear_changed(
		const scs_string_t name,
		const scs_u32_t index,
		const scs_value_t* const value,
		const scs_context_t context);

	/// <summary>
	/// Function that registers all telemetry handlers with SDK API.
	/// </summary>
	SCSAPI_RESULT register_handlers(const scs_telemetry_init_params_v101_t* const version_params, const scs_context_t context);
}