#pragma once

#include "config/config.h"
#include "service/pwm_service.h"

namespace service {
class MonitorService {
 public:
  MonitorService(const config::Config& config, const PwmService& pwm_service,
                 const int32_t& temperature)
      : config_(config), pwm_service_(pwm_service), temperature_(temperature) {}

  void Run();

 private:
  std::string GetStatusInfo() const;

  const config::Config& config_;
  const PwmService& pwm_service_;
  const int32_t& temperature_;
};
}  // namespace service