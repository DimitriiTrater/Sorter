#pragma once

#include "container/container.hpp"
#include "interfaces/isort.hpp"
#include "math/math.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

class TimeSort : public ISort {
public:
  virtual void sort(std::vector<Container> &vec_of_conts) override {
    std::sort(vec_of_conts.begin(), vec_of_conts.end(),
              [](Container a, Container b) {
                return a.time_of_creation > b.time_of_creation;
              });
  }
  virtual SORT_TYPE get_sort_type() override { return SORT_TYPE::TIME; }
};
