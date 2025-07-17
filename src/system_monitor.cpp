#include "system_monitor.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

float SystemMonitor::getCPUUsage() {
  std::ifstream file("/proc/stat");
  std::string line;
  std::getline(file, line); // First line with "cpu"
  std::istringstream iss(line);

  std::string cpu;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal;

  long idleTime = idle + iowait;
  long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  file.clear();
  file.seekg(0);            // Go back to beginning
  std::getline(file, line); // Read again
  iss.clear();
  iss.str(line);

  iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal;

  long idleTime2 = idle + iowait;
  long totalTime2 =
      user + nice + system + idle + iowait + irq + softirq + steal;

  float totalDiff = totalTime2 - totalTime;
  float idleDiff = idleTime2 - idleTime;

  return 100.0f * (1.0f - idleDiff / totalDiff);
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

void SystemMonitor::printStats() {
  float cpu = getCPUUsage();
  float mem = getMemoryUsage();

  std::cout << "CPU Usage: " << cpu << " %" << std::endl;
  std::cout << "Memory Usage: " << mem << " %" << std::endl;
}
