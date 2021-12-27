#pragma once

#include "game.hpp"
#include "graph_generator.hpp"
namespace uni_course_cpp {
class GameGenerator {
 public:
  explicit GameGenerator(
      const GraphGenerator::Params& params = GraphGenerator::Params())
      : params_(params) {}

  Game generate() const;

 private:
  const GraphGenerator::Params params_ = GraphGenerator::Params();
};
}  // namespace uni_course_cpp
