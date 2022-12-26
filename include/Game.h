#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <SDL2/SDL.h>

#include "World.h"
#include "Player.h"
#include "Building.h"
#include "Camera.h"
#include "InputManager.h"
#include "GLSLProgram.h"

enum GameState {
  RUNNING, STOPPED
};

class Game
{
 public:

  Game();
  ~Game();
  
  void initialize();
  void run();
  
 private:
  
  GameState gameState;
  
  const int screenWidth = 1200;
  const int screenHeight = 900;
  const int maxFps = 60;
  const float scrollSpeed = 0.02f;
  
  float fps;

  bool buildMode;
  
  SDL_Window* window;
  SDL_Renderer* renderer;

  std::vector<GLSLProgram> glslPrograms;
  GLuint uniformLocationMouse;
  
  World world;
  std::vector<Player*> players;
  std::vector<Building*> buildings;
  Building newBuilding;
  
  InputManager inputManager;
  
  void fatalError(const std::string&);
  
  void processInput();
  void installShaders();
  void drawScene();
  void updateFPS();
};

#endif
