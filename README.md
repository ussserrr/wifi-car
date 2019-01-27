# Wi-Fi car

The repo is a straightforward example of the [trackpad-joystick](https://github.com/ussserrr/trackpad-joystick) project. But it can be used with other controlling devices as well (both hardware and software (look at the [Blynk](https://www.blynk.cc))).


## Overview

The app is establishing a network connection and is waiting for the incoming UDP packets containing X, Y components of the instant movement (PWM value and servo angle in our case).


## Build and flash

Use [PlatformIO](https://platformio.org) to easily deploy the program. See corresponding docs for more information.
