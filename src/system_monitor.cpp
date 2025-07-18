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

float SystemMonitor::getMemoryUsage(float &totalMB, float &usedMB) {
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

  usedMB = (total - available) / 1024.0f;
  totalMB = total / 1024.0f;
  return 100.0f * usedMB / totalMB;
}

void drawBar(float percent, const std::string &label = "") {
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

  if (!label.empty()) {
    std::cout << "  " << label;
  }

  std::cout << std::endl;
}

void SystemMonitor::printStats() {
  float cpu = getCPUUsage();
  float totalMB, usedMB;
  float mem = getMemoryUsage(totalMB, usedMB);

  // Header
  std::cout << "\033[1;34m==============================\033[0m\n";
  std::cout << "         perfmon-tui\n";
  std::cout << "\033[1;34m==============================\033[0m\n\n";

  // Draw CPU and Memory bars with stats
  std::ostringstream cpuLabel;
  cpuLabel << std::fixed << std::setprecision(1)
           << (cpu / 100.0f * std::thread::hardware_concurrency()) << " / "
           << std::thread::hardware_concurrency() << " Threads";

  std::cout << "CPU Usage   ";
  drawBar(cpu, cpuLabel.str());

  std::ostringstream memLabel;
  memLabel << std::fixed << std::setprecision(1) << usedMB << " / " << totalMB
           << " MB";

  std::cout << "Memory Usage";
  drawBar(mem, memLabel.str());
}
