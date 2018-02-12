#include <iostream>
#include <memory>
#include <gamepad.hpp>



void button(uint16_t &buttons, int idx) {
  buttons ^= (uint16_t)(1 << idx);
}

int main() {
  std::unique_ptr<Gamepad> gamepad {new Gamepad{"First Virtual Gamepad", "SN 6666"}};
  char c;
  std::cout << "Input test command: q - quit, m - move left randomly, \
    0-9 down or up button u\n";

  gamepad_report report;

  while (c != 'q') {
    std::cin >> c;
    switch (c) {
      case 'm':
        report.left_x = rand();
        report.left_y = rand();
        gamepad->set_state(report);
        gamepad->send();
        break;
      case 'n':
          report.right_x = rand();
          report.right_y = rand();
          gamepad->set_state(report);
          gamepad->send();
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
