#include <csignal>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

#include "config/config.h"
#include "inih/INIReader.h"
#include "task.h"

int main(int argc, char** argv) {
  int ret = 0;
  auto& task = Task::GetInstance();

  // parse command and args
  if (argc != 3 || strcmp(argv[1], "-c") != 0) {
    std::cerr << "usage: " << argv[0] << " -c config_path" << std::endl;
    ret = 1;
    return ret;
  }

  // initialize config
  INIReader reader(argv[2]);
  if (reader.ParseError() < 0) {
    std::cerr << "Can't load " << argv[2] << std::endl;
    return 1;
  }
  ret = config::Config::Load(reader, task.GetCfg());
  if (ret != 0) {
    std::cerr << "load_config fail " << ret << std::endl;
    return ret;
  }
  std::cout << "loaded configuration: " << std::endl
            << task.GetCfg().ShortDebugString() << std::endl;

  // run task
  task.Run();
  std::cout << "task done." << std::endl;

  return 0;
}
