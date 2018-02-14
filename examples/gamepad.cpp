#include <iostream>
#include <memory>
#include <gamepad.hpp>



void button(uint16_t &buttons, int idx) {
  buttons ^= (uint16_t)(1 << idx);
}

int main() {
  std::unique_ptr<Gamepad> gamepad {new Gamepad{"First Virtual Gamepad", "SN 6666"}};
  char c;
  std::cout << "Input test command: q - quit, a,b,c,d,e,f - move axises randomly, \
    0-9 down or up button u\n";

  gamepad_report report;

  while (c != 'q') {
    std::cin >> c;
    switch (c) {
      case 'a':
        report.x = rand();
        gamepad->send_state(report);
        break;
      case 'b':
        report.y = rand();
        gamepad->send_state(report);
        break;
      case 'c':
        report.rx = rand();
        gamepad->send_state(report);
        break;
      case 'd':
        report.ry = rand();
        gamepad->send_state(report);
        break;
      case 'e':
        report.z = rand();
        gamepad->send_state(report);
        break;
      case 'f':
        report.rz = rand();
        gamepad->send_state(report);
        break;
      default:
        int idx = c - '0';
        if (idx >= 0 && idx <= 15) {
          std::cout << idx << std::endl;
          button(report.buttons, idx);
          std::cout << report.buttons << std::endl;
          gamepad->set_state(report);
          gamepad->send();
        }
        break;
    }
  }
}
