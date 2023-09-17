#pragma once

#include <iostream>
#include <string>
#include <vector>

void read_alphabet(std::vector<std::string> &out) {
  std::ifstream file("alphabet.txt");

  if (!file.is_open())
    return;
  out.clear();
  while (file) {
    std::string line{};
    file >> line;
    if (file.eof())
      break;
    out.push_back(line);
  }
  file.close();
}
