#pragma once
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include "graph_generator.hpp"

namespace uni_cpp_practice {
class GraphGenerationController {
 public:
  GraphGenerationController(int threads_count,
                            int graphs_count,
                            GraphGenerator::Params graph_generator_params);
  class Worker {
   public:
    using JobCallback = std::function<void()>;
    using GetJobCallback = std::function<std::optional<JobCallback>()>;
    using GenerateStartedCallback = std::function<void(int)>;
    using GenerateFinishedCallback = std::function<void(int, Graph)>;

    Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback) {}

    void start();
    void stop();

   private:
    std::thread thread_;
    bool terminate_flag = false;
    GetJobCallback get_job_callback_;
  };

  void generate(const Worker::GenerateStartedCallback& gen_started_callback,
                const Worker::GenerateFinishedCallback& gen_finished_callback);

  ~GraphGenerationController();

 private:
  int graphs_count_;
  GraphGenerator::Params params_;
  GraphGenerator graph_generator_;
  std::list<Worker> workers_;
  std::list<Worker::JobCallback> jobs_;
  std::mutex mutex_;
};
}  // namespace uni_cpp_practice
