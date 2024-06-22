#include "monitor_service.h"

#include <unistd.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace service {
void MonitorService::Run() {
  if (config_.common.monitor_log_path.empty()) {
    return;
  }

  namespace fs = std::filesystem;

  fs::path filename = config_.common.monitor_log_path;

  std::ofstream file(filename, std::ios::ate | std::ios::out);

  if (!file) {
    std::cerr << "Error opening file: " << filename << '\n';
    return;
  }

  while (true) {
    sleep(1);
    const auto status_info = GetStatusInfo();

    file.seekp(0);
    file << status_info << std::endl;
  }
}

std::string MonitorService::GetStatusInfo() const {
  std::stringstream ss;
  {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    ss << "update-time: " << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S")
       << std::endl;
  }
  ss << "temperature: " << temperature_ << std::endl;
  auto matched_rule = pwm_service_.GetMatchedRule();
  if (!matched_rule) {
    ss << "matched_rule: <empty>" << std::endl;
  } else {
    ss << matched_rule->ShortDebugString() << std::endl;
  }
  return ss.str();
}

}  // namespace service