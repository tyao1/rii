/*
  Define descriptors, datastructures and constant
*/
#include <IOKit/IOKitLib.h>
#include <iostream>
#include <memory>

#define SERVICE_NAME "it_unbit_foohid"
#define FOOHID_CREATE 0  // create selector
#define FOOHID_DESTROY 1  // destroy selector
#define FOOHID_SEND 2  // send selector

template<typename S, unsigned char* report_descriptor, uint64_t report_size>
class Device {
public:
  Device(
    const char* device_name, const char* device_sn
  ) {
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
    printf("Creating %s ...\n", device_name);
    // Create the device
    uint32_t input_count = 8;
    uint64_t input[input_count];
    input[0] = (uint64_t) strdup(device_name);  // device name
    input[1] = strlen((char *)input[0]);  // name length
    input[2] = (uint64_t) report_descriptor;  // report descriptor
    input[3] = report_size;  // report descriptor len
    input[4] = (uint64_t) strdup(device_sn);  // serial number
    input[5] = strlen((char *)input[4]);  // serial number len
    input[6] = (uint64_t) 2;  // vendor ID
    input[7] = (uint64_t) 3;  // device ID

    ret = IOConnectCallScalarMethod(
      connect_, FOOHID_CREATE, input, input_count, NULL, 0
    );
    if (ret != KERN_SUCCESS) {
       std::cout << "Unable to create HID device. May be fine if created previously.\n";
    }
    // fill in send_
    send_[0] = (uint64_t) strdup(device_name);  // device name
    send_[1] = strlen((char *)send_[0]);  // name length
    send_[2] = (uint64_t) &report_;  // report struct
    send_[3] = sizeof(S);  // report struct len
  }

  ~Device() {
    std::cout << "Destroying Device...\n";
    uint64_t data[2];
    data[0] = send_[0];
    data[1] = strlen((char *)send_[0]);
    IOConnectCallScalarMethod(
      connect_, FOOHID_DESTROY, data, 2, NULL, 0
    );
    IOServiceClose(connect_);
  }

  kern_return_t send() {
    kern_return_t res = IOConnectCallScalarMethod(
      connect_, FOOHID_SEND, send_, 4, NULL, 0
    );
    return res;
  }

  void set_state(const S new_state) {
    report_ = new_state;
  }

private:
  io_connect_t connect_;
  uint64_t send_[4];
  // const char* device_name_;
protected:
  S report_;
};
