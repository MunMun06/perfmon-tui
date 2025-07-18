#include "system_monitor.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

float SystemMonitor::getCPUUsage() {
  std::ifstream file("/proc/stat");
  std::string line;
  std::getline(file, line);
  std::istringstream iss(line);
  std::string cpu;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal;
  long idleTime1 = idle + iowait;
  long totalTime1 =
      user + nice + system + idle + iowait + irq + softirq + steal;

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  file.clear();
  file.seekg(0);
  std::getline(file, line);
  iss.clear();
  iss.str(line);
  iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal;
  long idleTime2 = idle + iowait;
  long totalTime2 =
      user + nice + system + idle + iowait + irq + softirq + steal;

  return 100.0f *
         (1.0f - (idleTime2 - idleTime1) / float(totalTime2 - totalTime1));
}

float SystemMonitor::getMemoryUsage() {
  std::ifstream file("/proc/meminfo");
  std::string line;
  long total = 0, available = 0;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string key;
    long value;
    std::string unit;
    iss >> key >> value >> unit;

    if (key == "MemTotal:")
      total = value;
    if (key == "MemAvailable:")
      available = value;
    if (total && available)
      break;
  }

  return 100.0f * (1.0f - (float)available / total);
}

void drawBar(float percent) {
  int width = 30;
  int filled = percent / 100.0f * width;

  std::cout << "[";
  for (int i = 0; i < width; ++i) {
    if (i < filled)
      std::cout << "\033[42m \033[0m"; // Green block
    else
      std::cout << " ";
  }
  std::cout << "] ";
  std::cout << std::fixed << std::setprecision(1) << percent << " %";
  std::cout << std::endl;
}

void SystemMonitor::printStats() {
  float cpu = getCPUUsage();
  float mem = getMemoryUsage();

  std::cout << "\033[1;34m==============================\033[0m\n";
  std::cout << "         perfmon-tui\n";
  std::cout << "\033[1;34m==============================\033[0m\n\n";

  std::cout << "CPU Usage   ";
  drawBar(cpu);
  std::cout << "Memory Usage";
  drawBar(mem);
}
