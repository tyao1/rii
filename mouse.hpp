#ifndef DEVICE_H
#define DEVICE_H
#include <device.hpp>
#endif

struct mouse_report {
   uint8_t buttons;
   int8_t x;
   int8_t y;
};

unsigned char report_descriptor[] = {
   0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
   0x09, 0x02,                    // USAGE (Mouse)
   0xa1, 0x01,                    // COLLECTION (Application)
   0x09, 0x01,                    //   USAGE (Pointer)
   0xa1, 0x00,                    //   COLLECTION (Physical)
   0x05, 0x09,                    //     USAGE_PAGE (Button)
   0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
   0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
   0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
   0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
   0x95, 0x03,                    //     REPORT_COUNT (3)
   0x75, 0x01,                    //     REPORT_SIZE (1)
   0x81, 0x02,                    //     INPUT (Data,Var,Abs)
   0x95, 0x01,                    //     REPORT_COUNT (1)
   0x75, 0x05,                    //     REPORT_SIZE (5)
   0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
   0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
   0x09, 0x30,                    //     USAGE (X)
   0x09, 0x31,                    //     USAGE (Y)
   0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
   0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
   0x75, 0x08,                    //     REPORT_SIZE (8)
   0x95, 0x02,                    //     REPORT_COUNT (2)
   0x81, 0x06,                    //     INPUT (Data,Var,Rel)
   0xc0,                          //   END_COLLECTION
   0xc0                           // END_COLLECTION
};

class Mouse: Device<mouse_report, report_descriptor, sizeof(report_descriptor)> {
public:
  Mouse(const char* device_name, const char* device_sn):Device(
    device_name, device_sn) {
  }

  kern_return_t move_mouse(int8_t x, int8_t y) {
    std::cout << (int)x << '\n' << (int)y << '\n';
    report_.x = x;
    report_.y = y;
    kern_return_t res = send();
    report_.x = 0;
    report_.y = 0;
    return res;
  }


  kern_return_t middle_down() {
    report_.buttons |= 1 << 2;
    return send();
  }

  kern_return_t middle_up() {
    report_.buttons &= ~(1 << 2);
    return send();

  }

  kern_return_t right_down() {
    report_.buttons |= 1 << 1;
    return send();
  }

  kern_return_t right_up() {
    report_.buttons &= ~(1 << 1);
    return send();
  }
  kern_return_t left_down() {
    report_.buttons |= 1;
    return send();
  }

  kern_return_t left_up() {
    report_.buttons &= ~1;
    return send();
  }

  int8_t getX() {
    return report_.x;
  }
  int8_t getY() {
    return report_.y;
  }
};
