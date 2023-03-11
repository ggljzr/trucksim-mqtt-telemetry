# trucksim-mqtt-telemetry

*Work in progress*

ATS/ETS2 plugin for publishing game telemetry via MQTT protocol.

## Current state

Basic proof of concept. I managed to get the MQTT client running from within the plugin. For fast updating channels
(like RPM or speed) its crucial to disable client persistence (by not setting the path to the persistence file).

Combination of no persistence, no message retaining, QoS=0 seems to work well enough with blocking client, even for 
fast changing channels, without noticable FPS drops.

However in the future it would be nice to implement some sort of restrictions about how many messages can one channel publish.
Especially world placement channel seems to publish a lot of messages that don't change much or at all. It would also be nice
to have some config file that will allow user to only register channels they need.

For now, I commented out some fast updating channels registration, since I don't plan to use them.

## Note about platform

This readme describies requirements and build process for ``x64`` target platform. If you want to target different platform
(i. e. ``x86``), you will need to make appropriate adjustments.

## Requirements

* Eclipse Paho MQTT [library](https://github.com/eclipse/paho.mqtt.cpp), 
* https://github.com/nlohmann/json JSON library.

Both can be installed with [vcpkg](https://vcpkg.io/en/index.html) (don't forget to specify the ``x64`` version):

```
> vcpkg install paho-mqttpp3:x64-windows
> vcpkg install nlohmann-json:x64-windows
```

## Building DLL file

Note that plugin DLL file is created in platform folder (``x64`` or ``x86``) within *solution* folder, not the ``trucksim-mqtt-telemetry`` subfolder.

Project release configuration defines a build event that installs the plugin into ATS. This event copies all dependencies into 
``C:\Program Files (x86)\Steam\steamapps\common\American Truck Simulator\bin\win_x64`` folder (to make sure they are correctly loaded)
and then copies plugin DLL into ``plugins`` subfolder. These paths have to exist for project to build properly.

If you dont intend to use ATS for plugin developement, you should modify or remove this build event.

Both debug and release configurations also define ``NOMINMAX`` flag for preprocessor. This is due to a [bug](https://github.com/eclipse/paho.mqtt.cpp/issues/226) in Paho MQTT library on Windows.

## Links

* [RenCloud](https://github.com/RenCloud/scs-sdk-plugin) telemetry plugin.