#ifndef SYSTEM_MONITOR_HPP
#define SYSTEM_MONITOR_HPP

class SystemMonitor {
public:
  void printStats();

private:
  float getCPUUsage();
  float getMemoryUsage(float &totalMB, float &usedMB);
};

#endif // SYSTEM_MONITOR_HPP
