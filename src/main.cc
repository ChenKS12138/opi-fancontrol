#include <csignal>
#include <iostream>
#include <thread>

#include "config/config.h"
#include "inih/INIReader.h"
#include "service/PwmService.h"
#include "service/TemperatureService.h"

using namespace config;
using namespace service;

class Task {
 public:
  static Task& GetInstance() {
    static Task task;
    return task;
  }
  explicit Task()
      : cfg_(),
        temperature_(0),
        pwm_service_(cfg_, temperature_),
        temperature_service_(cfg_, temperature_) {}
  void Run() {
    threads_.emplace_back([&]() { pwm_service_.Run(); });
    threads_.emplace_back([&]() { temperature_service_.Run(); });

    for (auto& t : threads_) {
      t.join();
    }
  }

  void OnStop(int signum) { pwm_service_.Stop(); }

  Config& GetCfg() { return cfg_; }

 private:
  Config cfg_;
  uint32_t temperature_;
  PwmService pwm_service_;
  TemperatureService temperature_service_;
  std::vector<std::thread> threads_;
};

void signalHandler(int signum) {
  std::cout << "Interrupt signal (" << signum << ") received. Exiting...\n";
  Task::GetInstance().OnStop(signum);
  exit(signum);
}

int main(int argc, char** argv) {
  int ret = 0;
  if (argc != 3 || strcmp(argv[1], "-c") != 0) {
    std::cerr << "usage: " << argv[0] << " -c config_path" << std::endl;
    ret = 1;
    return ret;
  }

  auto& task = Task::GetInstance();

  INIReader reader(argv[2]);
  if (reader.ParseError() < 0) {
    std::cerr << "Can't load " << argv[2] << std::endl;
    return 1;
  }
  ret = Config::Load(reader, task.GetCfg());
  if (ret != 0) {
    std::cerr << "load_config fail " << ret << std::endl;
    return ret;
  }

  std::cout << "loaded configuration: " << std::endl
            << task.GetCfg().ShortDebugString() << std::endl;

  signal(SIGINT, signalHandler);

  task.Run();
  std::cout << "All threads have finished." << std::endl;

  return 0;
}
