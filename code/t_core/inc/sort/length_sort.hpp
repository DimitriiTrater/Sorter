#pragma once
#include "container/container.hpp"
#include "interfaces/isort.hpp"
#include "math/math.hpp"
#include <algorithm>
#include <cmath>

class LengthSort : public ISort {
public:
  virtual void sort(std::vector<Container> &vec_of_conts) override {
    std::sort(
        vec_of_conts.begin(), vec_of_conts.end(),
        [](Container a, Container b) { return len_count(a) < len_count(b); });
  }
  virtual SORT_TYPE get_sort_type() override { return SORT_TYPE::LENGTH; }
};
