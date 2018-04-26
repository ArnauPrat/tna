

#include "engine/engine.h"
#include "game.h"
#include <cstdio>


int main(int argc, char** argv) {

  try {
  tna::initialize();
  tna::Game game(180,120);
  tna::run(&game);
  } catch(const std::runtime_error& error) {
    tna::log->error(error.what());
  } catch(...) {
    tna::log->error("Uncatched Exception!");
  }
  tna::terminate();
  return 0;
}

