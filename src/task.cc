#include "task.h"

#include <csignal>
#include <sstream>

void signalHandler(int signum) {
  std::cout << "Interrupt signal (" << signum << ") received. Exiting...\n";
  Task::GetInstance().OnStop(signum);
  exit(0);
}

void Task::Run() {
  signal(SIGINT, signalHandler);

  threads_.emplace_back([&]() { pwm_service_.Run(); });
  threads_.emplace_back([&]() { temperature_service_.Run(); });
  threads_.emplace_back([&]() { monitor_service_.Run(); });

  for (auto& t : threads_) {
    t.join();
  }
}
