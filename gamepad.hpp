#ifndef DEVICE_H
#define DEVICE_H
#include <device.hpp>
#endif

struct gamepad_report {
    int16_t x;
    int16_t y;
    int16_t rx;
    int16_t ry;
    uint8_t z;
    uint8_t rz;
    uint16_t buttons;
    void print() {
      printf("Buttons: %d, X1: %d, Y1: %d, X2: %d, Y2: %d\n",
        buttons, x, y, rx, ry);
    }
};

// http://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/
// Used HIDTool to generate the data
unsigned char gamepad_report_descriptor[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x33,                    //     USAGE (Rx)
    0x09, 0x34,                    //     USAGE (Ry)
    0x16, 0x00, 0x80,              //     LOGICAL_MINIMUM (-32768)
    0x26, 0xff, 0x7f,              //     LOGICAL_MAXIMUM (32767)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x32,                    //     USAGE (Z)
    0x09, 0x35,                    //     USAGE (Rz)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //     PHYSICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x10,                    //     REPORT_COUNT (16)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0xc0                           // END_COLLECTION
};


class Gamepad: public Device<gamepad_report, gamepad_report_descriptor, sizeof(gamepad_report_descriptor)> {
public:
  Gamepad(const char* device_name, const char* device_sn):Device(
    device_name, device_sn) {
  }
};
