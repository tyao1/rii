# About
Create virtual gamepads🎮  for macOS!  
The application will listen for UDP packet, and make real gamepad movements!  
Please refer to `server.cpp` and `gamepad.hpp` for detail

This is a user space application, that communicates with the [foohid driver](https://github.com/unbit/foohid)  
To run the application, the driver needs to be installed first.  

# Buck
[Buck](https://buckbuild.com) is used to build.
### Run
`buck run :server`

### Build
`buck build :server`

# Descriptor
Descriptor is created using [HID Descriptor Tool](http://www.usb.org/developers/hidpage#HID Descriptor Tool)  
16 buttons, and 2 thumbsticks, it's easy to customize.
