# trucksim-mqtt-telemetry

*Work in progress. Current state: basic proof of concept. I managed to get the MQTT client running from within the plugin.*

ATS/ETS2 plugin for publishing game telemetry via MQTT protocol.

## Requirements

Plugins uses Eclipse Paho MQTT [library](https://github.com/eclipse/paho.mqtt.cpp), which can be installed with
[VCPKG](https://vcpkg.io/en/index.html):

```
> vcpkg install paho-mqttpp3:x64-windows
```

## Building DLL file

Note that plugin DLL file is created in platform folder (``x64`` or ``x86``) within *solution* folder, not the ``trucksim-mqtt-telemetry`` subfolder.

Project release configuration defines a build event that installs the plugin into ATS. This event copies all dependencies into 
``C:\Program Files (x86)\Steam\steamapps\common\American Truck Simulator\bin\win_x64`` folder (to make sure they are correctly loaded)
and then copies plugin DLL into ``plugins`` subfolder. These paths have to exist for project to build properly.

If you dont intend to use ATS for plugin developement, you should modify or remove this build event.

Both debug and release configurations also define ``NOMINMAX`` flag for preprocessor. This is due to a [bug](https://github.com/eclipse/paho.mqtt.cpp/issues/226) in Paho MQTT library on Windows.