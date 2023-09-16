#include "math/math.hpp"
#include <cmath>

double len_count(double x, double y) {
  return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

double len_count(const Container &cont) {
  return len_count(cont.coord_x, cont.coord_y);
}
