#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "config.hpp"
#include "game.hpp"
#include "game_generator.hpp"
#include "game_printing.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_path.hpp"
#include "graph_printing.hpp"
#include "graph_traversal_controller.hpp"
#include "graph_traverser.hpp"
#include "logger.hpp"

namespace {
int handle_graphs_count_input() {
  int graph_count;
  std::cout << "Please, enter: how many graphs do you want: ";
  std::cin >> graph_count;
  while (graph_count < 0) {
    std::cout << "Please, try again: ";
    std::cin >> graph_count;
  }
  return graph_count;
}

int handle_depth_input() {
  int depth;
  std::cout << "Please, enter the depth of your graph: ";
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Negative depth, please, try again: ";
    std::cin >> depth;
  }
  return depth;
}

int handle_new_vertices_count_input() {
  int new_vertices_count;
  std::cout
      << "Please, enter count of new neighbors for each vertex of your graph: ";
  std::cin >> new_vertices_count;
  while (new_vertices_count < 0) {
    std::cout << "Negative new_vertices_count, please, try again: ";
    std::cin >> new_vertices_count;
  }
  return new_vertices_count;
}

int handle_threads_count_input() {
  int threads_count;
  std::cout << "Please, enter count of threads: ";
  std::cin >> threads_count;
  while (threads_count < 1 ||
         threads_count > std::thread::hardware_concurrency()) {
    std::cout << "Cant do this count of threads, please, try again: ";
    std::cin >> threads_count;
  }
  return threads_count;
}

void write_to_file(const std::string& graph_json,
                   const std::string& file_path) {
  std::ofstream out_file;
  out_file.open(std::string(config::kTempDirectoryPath) + file_path);
  out_file << graph_json;
  out_file.close();
}

std::string generation_started_string(const int graph_number) {
  std::stringstream log_string;
  log_string << "Graph " << graph_number << " Generation Started" << std::endl;
  return log_string.str();
}

std::string generation_finished_string(const int graph_number,
                                       const std::string& graph_description) {
  std::stringstream log_string;
  log_string << "Graph " << graph_number << " Generation Finished {"
             << std::endl;
  log_string << graph_description << std::endl << "}" << std::endl;
  return log_string.str();
}

std::string traversing_started_string(const int graph_number) {
  std::stringstream log_string;
  log_string << "Graph " << graph_number << " Traversing Started" << std::endl;
  return log_string.str();
}

std::string traversing_finished_string(
    const int graph_number,
    const std::vector<uni_course_cpp::GraphPath>& paths) {
  std::stringstream result_stream;
  result_stream << "Graph " << graph_number << ", Traversal Finished, Paths: ["
                << std::endl;
  for (const auto& path : paths) {
    result_stream << "  " << uni_course_cpp::printing::path_to_json(path);
    if (path.vertex_ids.back() != paths.back().vertex_ids.back())
      result_stream << ",";
    result_stream << std::endl;
  }

  result_stream << "]" << std::endl;
  return result_stream.str();
}

std::string game_preparing_string() {
  return "Game is Preparing...\n";
}

std::string game_ready_string(const uni_course_cpp::Game& game) {
  std::stringstream result;
  result << "Game is Ready {" << std::endl;
  result << uni_course_cpp::printing::print_game(game) << std::endl;
  return result.str();
}

std::string shortest_path_searching_string() {
  return "Searching for Shortest Path...\n";
}

std::string shortest_path_ready_string(
    const uni_course_cpp::GraphPath& shortest_path) {
  return "Shortest Path: " +
         uni_course_cpp::printing::path_to_json(shortest_path) + "\n";
}

std::string fastest_path_searching_string() {
  return "Searching for Fastest Path...\n";
}

std::string fastest_path_ready_string(
    const uni_course_cpp::GraphPath& fastest_path) {
  return "Fastest Path: " +
         uni_course_cpp::printing::path_to_json(fastest_path) + "\n";
}

void prepare_temp_directory() {
  std::filesystem::create_directory(config::kTempDirectoryPath);
}

std::vector<uni_course_cpp::Graph> generate_graphs(
    const uni_course_cpp::GraphGenerator::Params& params,
    int graphs_count,
    int threads_count) {
  auto generation_controller = uni_course_cpp::GraphGenerationController(
      threads_count, graphs_count, params);
  auto& logger = uni_course_cpp::Logger::get_logger();
  auto graphs = std::vector<uni_course_cpp::Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(generation_started_string(index)); },
      [&logger, &graphs](int index, uni_course_cpp::Graph graph) {
        const auto graph_description =
            uni_course_cpp::printing::print_graph(graph);
        logger.log(generation_finished_string(index, graph_description));
        const auto graph_json =
            uni_course_cpp::printing::json::print_graph(graph);
        graphs.push_back(std::move(graph));
        write_to_file(graph_json, "graph_" + std::to_string(index) + ".json");
      });

  return graphs;
}

void traverse_graphs(int threads_count,
                     const std::vector<uni_course_cpp::Graph>& graphs) {
  auto traversal_controller =
      uni_course_cpp::GraphTraversalController(threads_count, graphs);
  auto& logger = uni_course_cpp::Logger::get_logger();
  traversal_controller.traverse_graphs(
      [&logger](int index) { logger.log(traversing_started_string(index)); },
      [&logger](int index,
                const std::vector<uni_course_cpp::GraphPath>& paths) {
        logger.log(traversing_finished_string(index, paths));
      });
}

}  // namespace

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int threads_count = handle_threads_count_input();
  prepare_temp_directory();

  auto& logger = uni_course_cpp::Logger::get_logger();
  logger.log(game_preparing_string());

  const auto params =
      uni_course_cpp::GraphGenerator::Params(depth, new_vertices_count);
  const auto game_generator = uni_course_cpp::GameGenerator(params);
  const auto game = game_generator.generate();

  logger.log(game_ready_string(game));
  logger.log(shortest_path_searching_string());

  const auto shortest_path = game.find_shortest_path();

  logger.log(shortest_path_ready_string(shortest_path));
  logger.log(fastest_path_searching_string());

  const auto fastest_path = game.find_fastest_path();

  logger.log(fastest_path_ready_string(fastest_path));

  const auto map_json =
      uni_course_cpp::printing::json::print_map(game.get_map());
  write_to_file(map_json, "map.json");

  return 0;
}