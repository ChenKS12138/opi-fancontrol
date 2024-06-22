#pragma once

#include <cstdint>
#include <thread>

#include "config/config.h"
#include "inih/INIReader.h"
#include "service/monitor_service.h"
#include "service/pwm_service.h"
#include "service/temperature_service.h"

class Task {
 public:
  static Task& GetInstance() {
    static Task task;
    return task;
  }
  explicit Task()
      : cfg_(),
        temperature_(0),
        pwm_service_(cfg_, temperature_),
        temperature_service_(cfg_, temperature_),
        monitor_service_(cfg_, pwm_service_, temperature_) {}
  void Run();
  void OnStop(int signum) { pwm_service_.Stop(); }

  config::Config& GetCfg() { return cfg_; }

 private:
  config::Config cfg_;
  uint32_t temperature_;
  service::PwmService pwm_service_;
  service::TemperatureService temperature_service_;
  service::MonitorService monitor_service_;
  std::vector<std::thread> threads_;
};