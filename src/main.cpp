

#include "engine/engine.h"
#include "game.h"
#include <cstdio>


int main(int argc, char** argv) {

  tna::initialize();

  tna::Game game(180,120);
  tna::run(&game);
  tna::terminate();
  return 0;
}

