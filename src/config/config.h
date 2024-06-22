#pragma once

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "inih/INIReader.h"

namespace config {

struct Config {
  struct Common {
    std::string ShortDebugString() const;

    int32_t pin;
    int32_t rule_count;
    uint64_t pwm_duration_ns;
  };

  struct Rule {
    std::string ShortDebugString() const;

    uint32_t min_temp;
    uint32_t ratio;
  };

  static int Load(const INIReader& reader, Config& config);
  std::string ShortDebugString() const;

  Common common;
  std::vector<Rule> rules;
};

}  // namespace config
