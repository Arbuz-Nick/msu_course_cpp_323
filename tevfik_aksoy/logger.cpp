#include "logger.hpp"
#include <iostream>

void Logger::set_file(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    file_stream_->close();
    file_stream_ = std::nullopt;
    return;
  }

  if (file_stream_->is_open()) {
    file_stream_->close();
  }

  file_stream_ = std::ofstream(filename.value());
  if (!file_stream_->is_open())
    throw std::runtime_error("Error while opening the log file!");
}

void Logger::log(const std::string& string) {
  if (file_stream_.has_value())
    file_stream_.value() << string;
  std::cout << string;
}
