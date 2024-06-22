#include "config.h"
namespace config {

std::string Config::Common::ShortDebugString() const {
  std::stringstream ss;
  ss << "{ pin: " << pin << ", rule_count: " << rule_count
     << ", pwm_cycle_ns: " << pwm_cycle_ns << " }";
  return ss.str();
}

std::string Config::Rule::ShortDebugString() const {
  std::stringstream ss;
  ss << "{ min_temp: " << min_temp << ", ratio: " << ratio << " }";
  return ss.str();
}

int Config::Load(const INIReader& reader, Config& config) {
  auto& common = config.common;
  common.pin = reader.GetInteger("common", "pin", -1);
  common.rule_count = reader.GetInteger("common", "rule_count", -1);
  common.pwm_cycle_ns = reader.GetInteger("common", "pwm_cycle_ns", -1);

  auto& rules = config.rules;
  rules.clear();
  for (int i = 0; i < common.rule_count; i++) {
    Config::Rule rule;
    std::string section_name = "rule" + std::to_string(i);
    rule.min_temp = reader.GetInteger(section_name, "min_temp", 0);
    rule.ratio = reader.GetInteger(section_name, "ratio", 0);
    rules.push_back(std::move(rule));
  }

  return 0;
}

std::string Config::ShortDebugString() const {
  std::stringstream ss;
  ss << "{" << std::endl
     << "  "
     << "Common: " << common.ShortDebugString() << std::endl;
  for (int i = 0; i < rules.size(); i++) {
    ss << "  "
       << "Rule" << i << " " << rules[i].ShortDebugString() << std::endl;
  }
  ss << "}";
  return ss.str();
}

}  // namespace config
