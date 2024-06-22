#pragma once
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "config/config.h"

namespace service {

class TemperatureService {
 public:
  TemperatureService(const config::Config& config, int32_t& temperature)
      : config_(config), temperature_(temperature) {}
  void Run();

 private:
  void UpdateTemperature();

  const config::Config& config_;
  int32_t& temperature_;
};

}  // namespace service
