#pragma once
#include "container/container.hpp"
#include <fstream>
#include <optional>
#include <vector>

class FileManager {
public:
  [[nodiscard]] std::optional<std::vector<Container>> Read(std::string path);
  void Write(std::string str, std::ofstream::openmode openmode = std::ios::out);
  void Write(char fc, char sc, std::ofstream::openmode openmode = std::ios::out);
private:
  std::ifstream reader;
  std::ofstream writer;
};
