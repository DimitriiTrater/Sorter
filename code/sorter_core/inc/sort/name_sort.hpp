#pragma once

#include "container/container.hpp"
#include "interfaces/isort.hpp"
#include "math/math.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

class NameSort : public ISort {
public:
  virtual void sort(std::vector<Container> &vec_of_conts) override {
    std::sort(
        vec_of_conts.begin(), vec_of_conts.end(),
        [](const Container &a, const Container &b) { return a.name < b.name; });
  }
  virtual SORT_TYPE get_sort_type() override { return SORT_TYPE::NAME; }
};
