

#include "engine/engine.h"
#include "game.h"
#include <cstdio>


int main(int argc, char** argv) {

  tna::initialize();

  try {
  tna::Game game(180,120);
  tna::run(&game);
  } catch(std::runtime_error& error) {
    tna::log->error(error.what());
  } 
  tna::terminate();
  return 0;
}

