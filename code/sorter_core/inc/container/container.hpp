#pragma once
#include "string"

struct Container {
  std::string name{};
  double coord_x{};
  double coord_y{};
  std::string type{};
  double time_of_creation{};
};
