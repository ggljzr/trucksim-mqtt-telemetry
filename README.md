# trucksim-mqtt-telemetry

ATS/ETS2 plugin for publishing game telemetry via MQTT protocol.

## Performance
Combination of no persistence, no message retaining, QoS=0 and async client seems to work well enough, even for 
fast changing channels, without noticable FPS drops.

However in the future it would be nice to implement some sort of restrictions about how many messages can one channel publish.
Especially world placement channel seems to publish a lot of messages that don't change much or at all. It would also be nice
to have some config file that will allow user to only register channels they need.

For now, I commented out some fast updating channels registration, since I don't plan to use them.

## Note about platform

This readme describies requirements and build process for ``x64`` target platform. If you want to target different platform
(i. e. ``x86``), you will need to make appropriate adjustments.

## MQTT broker

You need an MQTT broker to run this plugin. I recommend running the [Mosquitto](https://mosquitto.org/) broker on the same machine
as the game itself, to minimize the latency. Other clients (subscribers) can connect to the broker from elsewhere (e. g. from LAN).

For now, the connection to the broker is hardcoded (defined in ``config.h``) and defaults to ``tcp://127.0.0.1:1883``. To change this
you need to recompile the plugin. I will hopefully add option to load configuration from a file some time soon.

## Requirements

* Eclipse Paho MQTT [library](https://github.com/eclipse/paho.mqtt.cpp), 
* https://github.com/nlohmann/json JSON library.

Both can be installed with [vcpkg](https://vcpkg.io/en/index.html) (don't forget to specify the ``x64`` version):

```
# v1.2.0#1
> vcpkg install paho-mqttpp3:x64-windows

# v3.11.2
> vcpkg install nlohmann-json:x64-windows 
```

## Building DLL file

Note that plugin DLL file is created in platform folder (``x64`` or ``x86``) within *solution* folder, not the ``trucksim-mqtt-telemetry`` subfolder.

Project release configuration defines a build event that installs the plugin into ATS. This event copies all dependencies into 
``C:\Program Files (x86)\Steam\steamapps\common\American Truck Simulator\bin\win_x64`` folder (to make sure they are correctly loaded)
and then copies plugin DLL into ``plugins`` subfolder. These paths have to exist for project to build properly.

If you dont intend to use ATS for plugin developement, you should modify or remove this build event.

Both debug and release configurations also define ``NOMINMAX`` flag for preprocessor. This is due to a [bug](https://github.com/eclipse/paho.mqtt.cpp/issues/226) in Paho MQTT library on Windows.

## Installation

You can find information on how to install precompiled plugin in the [latest release](https://github.com/ggljzr/trucksim-mqtt-telemetry/releases/latest).

## Examples

You can find basic examples on how to use MQTT API from Python or Javascript in the ``examples`` folder. For more complex stuff you can check:

* [trucksim-dashboard](https://github.com/ggljzr/trucksim-dashboard) -- Dashboard for displaying map location, job information, navigation time and distance, and time to next rest stop. This was the primary reason I made this plugin. Dashboard is written in React and can be used as a basic template on how to make similar applications.

## Documentation

You can find basic topic API description in the [docs]('docs/api.md').

## Links

* [RenCloud](https://github.com/RenCloud/scs-sdk-plugin) -- plugin with more complete API that uses in memory files for communication,
* [Route advisor app](https://github.com/mike-koch/ets2-mobile-route-advisor),
* [ATS/ETS2 map tiles](https://github.com/Unicor-p/SCS_Map_Tiles).