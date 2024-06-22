#pragma once

#include "config/config.h"
#include "wiringPi.h"

namespace service {
class PwmService {
 public:
  PwmService(const config::Config& config, uint32_t& temperature)
      : config_(config), temperature_(temperature), is_stop_(false) {}

  void Run();
  void Stop();

 private:
  using TimeSpec = struct timespec;

  void PinPrepare() {
    wiringPiSetup();
    pinMode(config_.common.pin, OUTPUT);
  }
  void PinWrite1() { digitalWrite(config_.common.pin, 1); }
  void PinWrite0() { digitalWrite(config_.common.pin, 0); }

  void DoValue0Duration(const config::Config::Rule* rule);
  void DoValue1Duration(const config::Config::Rule* rule);

  const config::Config::Rule* GetMatchedRule();

  int TimeSpecSleep(TimeSpec& req);

 private:
  const config::Config& config_;
  uint32_t& temperature_;

  bool is_stop_;
};
}  // namespace service