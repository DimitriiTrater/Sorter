#include "engine/engine.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
  auto &eng = Engine::getEngInst();
  return eng.run(argc, argv);
}
