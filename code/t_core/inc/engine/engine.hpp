#pragma once

#include "window/window.hpp"
#include <gtkmm.h>

class Engine {
  Engine() {}
  Engine(const Engine &);
  Engine &operator=(Engine &);

public:
  static Engine &getEngInst() {
    static Engine eng;
    return eng;
  }

  int run(int argc, char *argv[]) {
    auto app = Gtk::Application::create("t.x");
    return app->make_window_and_run<Window>(argc, argv);
  }
};
