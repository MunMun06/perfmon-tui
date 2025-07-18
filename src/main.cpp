#include "system_monitor.hpp"
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <thread>
#include <unistd.h>

void clearScreen() {
  std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top
}
void setNonBlockingInput(bool enable) {
  static struct termios oldt;
  struct termios newt;

  if (enable) {
    tcgetattr(STDIN_FILENO, &oldt); // save old settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // disable buffering & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking read
  } else {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore settings
  }
}

int main() {
  SystemMonitor monitor;
  setNonBlockingInput(true); // start raw input mode

  while (true) {
    clearScreen();
    monitor.printStats();

    std::cout << "\nPress 'q' to quit.\n";

    // Check if 'q' was pressed
    char ch;
    if (read(STDIN_FILENO, &ch, 1) > 0 && ch == 'q') {
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  setNonBlockingInput(false); // restore terminal mode
  return 0;
}
