
#include <IOKit/IOKitLib.h>
#include <iostream>
#include <memory>
#include <mouse.hpp>




int main() {
  std::unique_ptr<Mouse> mouse {new Mouse{"Da Virtual Mouse", "SN 666"}};
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
