#include "system_monitor.hpp"
#include <chrono>
#include <iostream>
#include <thread>

void clearScreen() {
  std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top
}

int main() {
  SystemMonitor monitor;

  while (true) {
    clearScreen();
    monitor.printStats();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
