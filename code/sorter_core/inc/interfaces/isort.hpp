#pragma once
#include "container/container.hpp"
#include <vector>

enum class SORT_TYPE { NOTHING = 0, LENGTH, NAME, TIME, TYPE };

class ISort {
public:
  virtual void sort(std::vector<Container> &vec_of_conts) = 0;
  virtual SORT_TYPE get_sort_type() { return SORT_TYPE::NOTHING; }
};
