#pragma once
#include "container/container.hpp"
#include <fstream>
#include <optional>
#include <vector>

class FileManager {
public:
  std::optional<std::vector<Container>> Read(std::string path);
  void Write();

private:
  std::ifstream reader;
  std::ofstream writer;
};
