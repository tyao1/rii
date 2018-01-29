/**
* Create a virtual mouse!
*/

#include <IOKit/IOKitLib.h>
#include <iostream>
#include <memory>
#include <oldmouse.hpp>

VirtualMouse::VirtualMouse() {
  std::cout << "Initializing the connection to the driver...\n";
  io_iterator_t iterator;
  io_service_t service;

  // Get a reference to the IOService
  kern_return_t ret = IOServiceGetMatchingServices(
    kIOMasterPortDefault, IOServiceMatching(SERVICE_NAME), &iterator
  );

  if (ret != KERN_SUCCESS) {
     throw "Unable to access IOService";
  }

  // looking for an available connection
  bool found = false;
  while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
     ret = IOServiceOpen(service, mach_task_self(), 0, &connect_);
     if (ret == KERN_SUCCESS) {
         found = true;
         break;
     }
     IOObjectRelease(service);
  }
  IOObjectRelease(iterator);

  if (!found) {
    throw "Unable to access IOService";
  }

  // Create the device
  uint32_t input_count = 8;
  uint64_t input[input_count];
  input[0] = (uint64_t) strdup(DEVICE_NAME);  // device name
  input[1] = strlen((char *)input[0]);  // name length
  input[2] = (uint64_t) report_descriptor;  // report descriptor
  input[3] = sizeof(report_descriptor);  // report descriptor len
  std::cout<<input[3]<<std::endl;

  input[4] = (uint64_t) strdup(DEVICE_SN);  // serial number
  input[5] = strlen((char *)input[4]);  // serial number len
  input[6] = (uint64_t) 2;  // vendor ID
  input[7] = (uint64_t) 3;  // device ID

  ret = IOConnectCallScalarMethod(
    connect_, FOOHID_CREATE, input, input_count, NULL, 0
  );
  if (ret != KERN_SUCCESS) {
     std::cout << "Unable to create HID device. May be fine if created previously.\n";
  }
  // init send data
  // have to dup the string to send to kernel space
  send_[0] = (uint64_t) strdup(DEVICE_NAME);  // device name
  send_[1] = strlen((char *)send_[0]);  // name length
  send_[2] = (uint64_t) &report_;  // mouse struct
  send_[3] = sizeof(mouse_report);  // mouse struct len
}

VirtualMouse::~VirtualMouse() {
  std::cout << "Destroying mouse...\n";
  uint64_t data[2];
  data[0] = (uint64_t) strdup(DEVICE_NAME);
  data[1] = strlen((char *)send_[0]);
  IOConnectCallScalarMethod(
    connect_, FOOHID_DESTROY, data, 2, NULL, 0
  );
  IOServiceClose(connect_);
}

kern_return_t VirtualMouse::move_mouse(int8_t x, int8_t y) {
  std::cout << (int)x << '\n' << (int)y << '\n';
  report_.x = x;
  report_.y = y;
  kern_return_t res = IOConnectCallScalarMethod(
    connect_, FOOHID_SEND, send_, 4, NULL, 0
  );
  report_.x = 0;
  report_.y = 0;
  return res;
}


kern_return_t VirtualMouse::middle_down() {
  report_.buttons |= 1 << 2;
  return IOConnectCallScalarMethod(
    connect_, FOOHID_SEND, send_, 4, NULL, 0
  );
}

kern_return_t VirtualMouse::middle_up() {
  report_.buttons &= ~(1 << 2);
  return IOConnectCallScalarMethod(
    connect_, FOOHID_SEND, send_, 4, NULL, 0
  );
}

kern_return_t VirtualMouse::right_down() {
  report_.buttons |= 1 << 1;
  return IOConnectCallScalarMethod(
    connect_, FOOHID_SEND, send_, 4, NULL, 0
  );
}

kern_return_t VirtualMouse::right_up() {
  report_.buttons &= ~(1 << 1);
  return IOConnectCallScalarMethod(
    connect_, FOOHID_SEND, send_, 4, NULL, 0
  );
}
kern_return_t VirtualMouse::left_down() {
  report_.buttons |= 1;
  return IOConnectCallScalarMethod(
    connect_, FOOHID_SEND, send_, 4, NULL, 0
  );
}

kern_return_t VirtualMouse::left_up() {
  report_.buttons &= ~1;
  return IOConnectCallScalarMethod(
    connect_, FOOHID_SEND, send_, 4, NULL, 0
  );
}


int8_t VirtualMouse::getX() {
  return report_.x;
}
int8_t VirtualMouse::getY() {
  return report_.y;
}

/*
  Test the mouse class
*/
int main() {
  std::unique_ptr<VirtualMouse> mouse {new VirtualMouse};
  char c;
  std::cout << "Input test command: q - quit, m - move mouse randomly, d - left down, u - left u\n";
  while (c != 'q') {
    std::cin >> c;
    switch (c) {
      case 'm':
        mouse->move_mouse(rand(), rand());
        break;
      case 'd':
        mouse->left_down();
        break;
      case 'u':
        mouse->left_up();
        break;
      case 'a':
        mouse->middle_down();
        break;
      case 'b':
        mouse->middle_up();
        break;
      case 'e':
        mouse->right_down();
        break;
      case 'f':
        mouse->right_up();
      default:
        break;
    }
  }
}
