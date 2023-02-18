# AirTop Front Panel daemon and GPU Thermal daemon

This project consists of the following parts:

- **airtop-fpsvc** - AirTop Front Panel service daemon. This service provides hardware-related metrics to the AirTop front panel controller.
- **gpu-thermald** - GPU Thermal daemon. This service sets GPU temperature upper limit by constantly controlling its power.

Tested with AirTop1 under distro:

- Linux Mint 20.3 Una base: Ubuntu 20.04 focal
- Linux Mint 21.1 Vera base: Ubuntu 22.04 jammy

## Building manually

Issue `make` in project top directory (where Makefile is present).
For debugging purposes, issue `make debug=1`.

'_obj_' directory will hold object and other temporary files, '_bin_' directory will hold the executables.

Install build dependencies:

`sudo apt-get install libsensors-dev libatasmart-dev libi2c-dev`

### Package build

`dpkg-buildpackage -b -uc -tc`

## Configuration

After installation, either by manual building or from package, you may configure airtop-fpsvc and gpu-thermald service.

Edit /etc/default/airtop-fpsvc and /etc/default/gpu-thermald to set the service options.

## Supported features status

- CPU Temperature
- CPU Frequency
- GPU Temperature
- HDD Temperature and volume - currently there is no FP request for HDD volume
- Limiting GPU power and/or temperature - GPU Thermal daemon

## Software design overview

### AirTop Front Panel Service daemon

This daemon is quite robust and scalable in the expense of simplicity of implementation. The daemon is built around 2 thread pools - frontend and backend.

- Frontend is a single-threaded 'owner' of the communication to and from the Front Panel controller. It accepts requests from the FP, and sends back the responses.

- Backend is a multi-threaded pool performing the tasks dispatched by the frontend. Data acquired by the backend is handed over to frontend to be passed on to the FP controller.

Each thread is guarded by a watchdog, so that no task can spend in the processing more than a preset time. Watchdog timeout is considered a major failure, and leads to daemon restart.

Notice, that this design enables adding additional frontends, e.g. a web-based one, that would allow creation of web-based front panel useful for system administration.

### GPU Thermal daemon

This daemon implements a modified PID control loop, limiting the GPU temperature by timely limiting of GPU power.
While the hard-coded PID factors were matched to meet AirTop case heat dissipation, the whole implementation is not AirTop-specific, and is able of serving any Nvidia GPU capable of power management.
