#pragma once

#include "config/config.h"
#include "wiringPi.h"

namespace service {
class PwmService {
 public:
  PwmService(const config::Config& config, int32_t& temperature)
      : config_(config),
        temperature_(temperature),
        matched_rule_(nullptr),
        is_stop_(false) {}

  void Run();
  void Stop();
  const config::Config::Rule* GetMatchedRule() const { return matched_rule_; }

 private:
  using TimeSpec = struct timespec;

  void PinPrepare() {
    wiringPiSetup();
    pinMode(config_.common.pin, OUTPUT);
  }
  void PinWrite1() { digitalWrite(config_.common.pin, 1); }
  void PinWrite0() { digitalWrite(config_.common.pin, 0); }

  void DoValue0Duration();
  void DoValue1Duration();

  void UpdateMatchedRule();

  int TimeSpecSleep(TimeSpec& req);

 private:
  const config::Config& config_;
  int32_t& temperature_;
  const config::Config::Rule* matched_rule_;

  bool is_stop_;
};
}  // namespace service