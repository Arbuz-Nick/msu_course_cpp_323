#include "graph_generator.hpp"
#include <atomic>
#include <cassert>
#include <functional>
#include <limits>
#include <list>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

namespace {

constexpr int MAX_THREADS_COUNT = 4;

using uni_cpp_practice::Depth;
using uni_cpp_practice::Edge;
using uni_cpp_practice::Graph;
using uni_cpp_practice::Vertex;
using uni_cpp_practice::VertexId;
using Params = uni_cpp_practice::GraphGenerator::Params;

double get_color_probability(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Green:
      return 0.1;
    case Edge::Color::Blue:
      return 0.25;
    case Edge::Color::Red:
      return 0.33;
    case Edge::Color::Yellow:
      return 1.0;
    case Edge::Color::Gray:
      return 1.0;
  }
}

bool is_lucky(double probability) {
  assert(probability + std::numeric_limits<double>::epsilon() >= 0 &&
         probability - std::numeric_limits<double>::epsilon() <= 1.0 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

int get_random_number(int size) {
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> distrib(0, size - 1);
  return distrib(gen);
}

void generate_green_edges(Graph& graph, std::mutex& mutex_add_edge) {
  const double probability = get_color_probability(Edge::Color::Green);
  for (const auto& [current_vertex_id, current_vertex] :
       graph.get_vertex_map()) {
    if (is_lucky(probability)) {
      const std::lock_guard lock(mutex_add_edge);
      graph.add_edge(current_vertex_id, current_vertex_id, Edge::Color::Green);
    }
  }
}

void generate_blue_edges(Graph& graph) {
  const double probability = get_color_probability(Edge::Color::Blue);
  // так как на нулевом уровне только одна вершина == нулевая, нет смысла ее
  // учитывать
  for (Depth current_depth = 1; current_depth <= graph.get_depth();
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    for (int idx = 0; idx < vertices_at_depth.size() - 1; ++idx) {
      if (is_lucky(probability)) {
        graph.add_edge(vertices_at_depth[idx], vertices_at_depth[idx + 1],
                       Edge::Color::Blue);
      }
    }
  }
}

void generate_yellow_edges(Graph& graph, std::mutex& mutex_add_edge) {
  double probability =
      get_color_probability(Edge::Color::Yellow) / (graph.get_depth() - 1);
  double yellow_edge_probability = probability;
  //так как вероятность генерации желтых ребер из нулевой вершины должна быть
  //нулевой, то можно просто не рассматривать эту вершину
  for (Depth current_depth = 1; current_depth < graph.get_depth();
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    const auto& vertices_at_next_depth =
        graph.get_vertices_at_depth(current_depth + 1);
    for (const auto& current_vertex_id : vertices_at_depth) {
      if (is_lucky(yellow_edge_probability)) {
        std::vector<VertexId> not_binded_vertices;
        for (const auto& next_vertex_id : vertices_at_next_depth) {
          if (!graph.check_binding(current_vertex_id, next_vertex_id)) {
            not_binded_vertices.push_back(next_vertex_id);
          }
        }
        if (not_binded_vertices.size()) {
          const int idx = get_random_number(not_binded_vertices.size());
          const std::lock_guard lock(mutex_add_edge);
          graph.add_edge(current_vertex_id, not_binded_vertices[idx],
                         Edge::Color::Yellow);
        }
      }
    }
    yellow_edge_probability += probability;
  }
}

void generate_red_edges(Graph& graph, std::mutex& mutex_add_edge) {
  const double probability = get_color_probability(Edge::Color::Red);
  for (Depth current_depth = 0; current_depth < graph.get_depth() - 1;
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    const auto& vertices_at_next_depth =
        graph.get_vertices_at_depth(current_depth + 2);
    for (const auto& current_vertex_id : vertices_at_depth) {
      if (is_lucky(probability)) {
        const int index = get_random_number(vertices_at_next_depth.size());
        const std::lock_guard lock(mutex_add_edge);
        graph.add_edge(current_vertex_id, vertices_at_next_depth[index],
                       Edge::Color::Red);
      }
    }
  }
}

void generate_gray_branch(Graph& graph,
                          const Params& params,
                          std::mutex& mutex_add,
                          const VertexId& parent_vertex_id,
                          const Depth current_depth,
                          double probability) {
  auto lambda = [&graph = graph, &mutex_add = mutex_add,
                 &parent_vertex_id = parent_vertex_id]() -> const VertexId {
    const std::lock_guard lock(mutex_add);
    const auto& new_vertex_id = graph.add_vertex();
    graph.add_edge(parent_vertex_id, new_vertex_id);
    return new_vertex_id;
  };
  const auto new_vertex_id = lambda();

  double new_vertex_probability =
      probability - current_depth * (probability / params.depth);
  if (current_depth < params.depth) {
    for (int i = 0; i < params.new_vertices_num; ++i) {
      if (is_lucky(new_vertex_probability)) {
        generate_gray_branch(graph, params, mutex_add, new_vertex_id,
                             current_depth + 1, probability);
      }
    }
  }
}

void generate_gray_edges(Graph& graph,
                         const Params& params,
                         const VertexId& parent_vertex_id) {
  // Job - это lambda функция,
  // которая энкапсулирует в себе генерацию однйо ветви
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  // Заполняем список работ для воркеров
  std::atomic<int> jobs_counter = 0;
  std::mutex mutex_add;
  double probability = get_color_probability(Edge::Color::Gray);
  Depth current_depth = 0;
  for (int i = 0; i < params.new_vertices_num; i++) {
    jobs.emplace_back([&graph = graph, &params = params, &mutex_add = mutex_add,
                       &jobs_counter = jobs_counter,
                       &parent_vertex_id = parent_vertex_id, current_depth,
                       probability]() {
      generate_gray_branch(graph, params, mutex_add, parent_vertex_id,
                           current_depth + 1, probability);
      ++jobs_counter;
    });
  }

  // Создаем воркера,
  // который в бесконечном цикле проверяет,
  // есть ли работа, и выполняет её
  std::mutex mutex_jobs;
  std::atomic<bool> should_terminate = false;
  auto worker = [&should_terminate = should_terminate, &mutex_jobs = mutex_jobs,
                 &jobs = jobs]() {
    while (true) {
      // Проверка флага, должны ли мы остановить поток
      if (should_terminate) {
        return;
      }
      // Проверяем, есть ли для нас работа
      const auto job_optional = [&mutex_jobs = mutex_jobs,
                                 &jobs = jobs]() -> std::optional<JobCallback> {
        const std::lock_guard lock(mutex_jobs);
        if (jobs.empty()) {
          return std::nullopt;
        }
        const auto fist_job = jobs.front();
        jobs.pop_front();
        return fist_job;
      }();
      if (job_optional.has_value()) {
        // Работа есть, выполняем её
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  // Создаем и запускаем потоки с воркерами
  // MAX_THREADS_COUNT = 4
  std::vector<std::thread> threads;
  for (int i = 0; i < MAX_THREADS_COUNT; ++i) {
    threads.emplace_back(std::thread(worker));
  }

  // Ждем, когда все ветви будут сгенерированы
  while (jobs_counter < params.new_vertices_num) {
  }

  // Останавливем всех воркеров и потоки
  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}
}  // namespace

namespace uni_cpp_practice {

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  const VertexId& new_vertex_id = graph.add_vertex();
  generate_gray_edges(graph, params_, new_vertex_id);
  std::mutex mutex_add_edge;
  std::thread green_thread(generate_green_edges, std::ref(graph),
                           std::ref(mutex_add_edge));
  std::thread yellow_thread(generate_yellow_edges, std::ref(graph),
                            std::ref(mutex_add_edge));
  std::thread red_thread(generate_red_edges, std::ref(graph),
                         std::ref(mutex_add_edge));
  green_thread.join();
  yellow_thread.join();
  red_thread.join();
  generate_blue_edges(graph);
  return graph;
}
}  // namespace uni_cpp_practice
