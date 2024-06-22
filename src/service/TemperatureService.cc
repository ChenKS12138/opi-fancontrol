#include "TemperatureService.h"

namespace service {

void TemperatureService::Run() {
  while (true) {
    UpdateTemperature();

    sleep(1);
  }
}

void TemperatureService::UpdateTemperature() {
  std::string tempFile = "/sys/class/thermal/thermal_zone0/temp";
  std::ifstream tempStream(tempFile);

  if (!tempStream.is_open()) {
    std::cerr << "Failed to open temperature file: " << tempFile << std::endl;
    return;
  }

  std::string tempStr;
  getline(tempStream, tempStr);
  tempStream.close();

  temperature_ = stof(tempStr) /
                 1000.0;  // Convert millidegrees Celsius to degrees Celsius
}
}  // namespace service