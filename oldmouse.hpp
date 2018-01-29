/*
  Define descriptors, datastructures and constant
*/

#include <IOKit/IOKitLib.h>

/*
  Reporter for a mouse
*/
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

struct mouse_report {
   uint8_t buttons;
   int8_t x;
   int8_t y;
};

#define SERVICE_NAME "it_unbit_foohid"

#define FOOHID_CREATE 0  // create selector
#define FOOHID_DESTROY 1  // destroy selector
#define FOOHID_SEND 2  // send selector

#define DEVICE_NAME "Foohid Virtual Mouse"
#define DEVICE_SN "SN 123456"

class VirtualMouse {
public:
  VirtualMouse();
  ~VirtualMouse();
  kern_return_t move_mouse(int8_t x, int8_t y);
  kern_return_t left_down();
  kern_return_t left_up();
  kern_return_t middle_down();
  kern_return_t middle_up();
  kern_return_t right_down();
  kern_return_t right_up();
  int8_t getX();
  int8_t getY();
private:
  io_connect_t connect_;
  mouse_report report_;
  uint64_t send_[4];
};
