#include "random.hpp"

#include <random>
#include <vector>
#include "graph.hpp"

namespace uni_course_cpp {
namespace graph_generating {
bool can_generate_vertex(float probability) {
  std::random_device random_device;
  std::mt19937 generate(random_device());
  std::bernoulli_distribution distribution(probability);
  return distribution(generate);
}

Graph::VertexId get_random_vertex_id(
    const std::vector<Graph::VertexId>& vertex_ids) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(0, vertex_ids.size() - 1);
  return vertex_ids[distribution(generator)];
}
}  // namespace graph_generating

int get_random_between(int start, int stop) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(start, stop);
  return distribution(generator);
}
}  // namespace uni_course_cpp
