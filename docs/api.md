# MQTT API

This documents describes topics currently implemented by the plugin. Note that the structure of topics and
format of responses tries to copy the original SDK API.

## Utility topics

### Log

**Topic:** ``trucksim/log``

Used for logging messages.

**Response:**

```javascript
{
	msg: string,
	// Log level:
	// Debug = 10,
	// Info = 20,
	// Warning = 30,
	// Error = 40
	level: number
}
```

### Game info

**Topic:** ``trucksim/gameinfo``

First message sent when the SDK is initialized. Contains info about game version and game ID (``ats`` for American Truck Simulator,
``eut2`` for Euro Truck Simulator 2).

If the game is exited, ``'null'`` string message is sent to this topic.

**Response:**

```javascript
{
	// "ats" or "eut2"
    game_id: string,
    game_version: number,
}
```

## Config events

### Truck event

**Topic:** ``trucksim/event/config/truck``

Topic for truck config events. ``'null'`` string is published to this topic to indicate that no truck is selected.

**Response:**

```javascript
{
    adblue_capacity: number,
    adblue_warning_factor: number,
    battery_voltage_warning: number,
    brake_air_pressure_emergency: number,
    brake_air_pressure_warning: number,
    brand: string,
    brand_id: string,
    cabin_position: {
        x: number,
        y: number,
        z: number
    },
    differential_ratio: number,
    forward_ratio: number,
    fuel_capacity: number,
    fuel_warning_factor: number,
    gears_forward: number,
    gears_reverse: number,
    head_position: {
        x: number,
        y: number,
        z: number
    },
    hook_position: {
        x: number,
        y: number,
        z: number
    },
    id: string,
    license_plate: string,
    license_plate_country: string,
    license_plate_country_id: string,
    name: string,
    oil_pressure_warning: number,
    retarder_steps: number,
    reverse_ratio: number,
    rpm_limit: number,
    water_temperature_warning: number,
    wheel_position: {
        x: number,
        y: number,
        z: number
    },
    wheel_powered: number,
    wheel_radius: number,
    wheel_simulated: number,
    wheel_steerable: number,
    wheels_count: number,
}
```

### Job event

**Topic:** ``trucksim/event/config/job``

Topic for job events. ``'null'`` string is published to this topic to indicate that no job is selected (e. g. when delivery has been finished).

**Response:**

```javascript
{
    cargo: string,
    cargo_id: string,
    cargo_loaded: boolean,
    cargo_mass: number,
    cargo_unit_count: number,
    cargo_unit_mass: number,
    delivery_time: number,
    destination_city: string,
    destination_city_id: string,
    destination_company: string,
    destination_company_id: string,
    income: number,
    is_special_job: boolean,
    job_market: string,
    planned_distance_km: number,
    source_city: string,
    source_city_id: string,
    source_company: string,
    source_company_id: string,
}
```

## Channels

### World placement

**Topic:** ``trucksim/channel/truck/world/placement``

This topic publishes information about trucks world placement. Topic is limited to at most one message each 100 ms.

**Response:**

```javascript
{
    x: number,
    y: number,
    z: number,
    heading: number,
    pitch: number,
    roll: number,
}
```

### Value channels

Each of this topic publishes only single value of given type. Message with the value has a following format:

```javascript
{ value: any }
```

These are currently implemented value channels:

* ``trucksim/channel/game/time`` - current ingame time. This is a number of minutes since the game starting point, which is on Monday, 00:00 - ``number``.
* ``trucksim/channel/rest/stop`` - minutes to the next rest stop - ``number``.
* ``trucksim/channel/truck/cruise_control`` - current cruise controll settings, 0 if disabled - ``number``.
* ``trucksim/channel/truck/engine/gear`` - current transmission gear, 0 for neutral, negative for reverse gears - ``number``.
* ``trucksim/channel/truck/fuel/amount`` - current fuel amount in liters - ``number``.
* ``trucksim/channel/truck/fuel/range`` - estimated range with current fuel in km - ``number``.
* ``trucksim/channel/truck/fuel/warning`` - low fuel warning - ``bool``.
* ``trucksim/channel/truck/navigation/distance`` - navigation distance (the distance to the navigation point set via world map or current job destination) in meters - ``number``.
* ``trucksim/channel/truck/navigation/time`` - navigation ETA (see navigation distance above) in seconds.
* ``trucksim/channel/truck/navigation/speed/limit`` - current speed limit, as displayed in the ingame navigation, in m/s - ``number``.