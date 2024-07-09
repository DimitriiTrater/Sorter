#pragma once

#include "window/window.hpp"
#include <QApplication>

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
    QApplication app(argc, argv);
    QString param = QString(argv[0]);
    Window w(nullptr);
    w.show();

    return app.exec();
  }
};
