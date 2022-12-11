#include <iostream>

#include "Game.h"

struct A {int a,b;};
int main()
{
  Game game;
  game.initialize();
  game.run();    
    
  return 0;
}
