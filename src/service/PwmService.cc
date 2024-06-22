#include "PwmService.h"

#include <time.h>
#include <unistd.h>

namespace service {
void PwmService::Run() {
  PinPrepare();
  while (true) {
    if (is_stop_) {
      return;
    }
    auto rule = GetMatchedRule();
    DoValue1Duration(rule);
    DoValue0Duration(rule);
  }
}
void PwmService::Stop() {
  is_stop_ = true;
  PinWrite0();
}

void PwmService::DoValue1Duration(const config::Config::Rule* rule) {
  PinWrite1();

  static TimeSpec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = config_.common.pwm_duration_ns / 100 *
               (rule == nullptr ? 0 : rule->ratio);
  TimeSpecSleep(ts);
}

void PwmService::DoValue0Duration(const config::Config::Rule* rule) {
  PinWrite0();

  static TimeSpec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = config_.common.pwm_duration_ns / 100 *
               (100 - (rule == nullptr ? 0 : rule->ratio));
  TimeSpecSleep(ts);
}

const config::Config::Rule* PwmService::GetMatchedRule() {
  for (int i = config_.rules.size() - 1; i >= 0; i--) {
    const auto& rule = config_.rules[i];
    if (temperature_ >= rule.min_temp) {
      return &rule;
    }
  }
  return nullptr;
}

int PwmService::TimeSpecSleep(TimeSpec& req) {
  static TimeSpec rem;

  if (req.tv_sec == 0 && req.tv_nsec == 0) {
    return 0;
  }

  // ensure full sleep time
  while (nanosleep(&req, &rem) == -1) {
    if (errno == EINTR) {
      req = rem;
    } else {
      perror("nanosleep");
      return 1;
    }
  }
  return 0;
}

}  // namespace service