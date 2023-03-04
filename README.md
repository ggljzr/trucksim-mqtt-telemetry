# trucksim-mqtt-telemetry

*Work in progress*

ATS/ETS2 plugin for publishing game telemetry via MQTT protocol.

## Building DLL file

Note that plugin DLL file is created in platform folder (``x64`` or ``x86``) within *solution* folder, not the ``trucksim-mqtt-telemetry`` subfolder.

Project release configuration has build event that copies the resulting DLL file directly into ATS plugin folder (``C:\Program Files (x86)\Steam\steamapps\common\American Truck Simulator\bin\win_x64\plugins``).
Make sure that this folder exists or remove this build event in project properties.