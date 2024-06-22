#include "pwm_service.h"

#include <time.h>
#include <unistd.h>

namespace service {
void PwmService::Run() {
  PinPrepare();
  while (true) {
    if (is_stop_) {
      return;
    }
    UpdateMatchedRule();
    DoValue1Duration();
    DoValue0Duration();
  }
}
void PwmService::Stop() {
  is_stop_ = true;
  PinWrite0();
}

void PwmService::DoValue1Duration() {
  PinWrite1();

  static TimeSpec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = config_.common.pwm_cycle_ns / 100 *
               (matched_rule_ == nullptr ? 0 : matched_rule_->ratio);
  TimeSpecSleep(ts);
}

void PwmService::DoValue0Duration() {
  PinWrite0();

  static TimeSpec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = config_.common.pwm_cycle_ns / 100 *
               (100 - (matched_rule_ == nullptr ? 0 : matched_rule_->ratio));
  TimeSpecSleep(ts);
}

void PwmService::UpdateMatchedRule() {
  for (int i = config_.rules.size() - 1; i >= 0; i--) {
    const auto& rule = config_.rules[i];
    if (temperature_ >= rule.min_temp) {
      matched_rule_ = &rule;
      return;
    }
  }
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