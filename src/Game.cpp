#include "Game.h"

#include <cmath>
#include <vector>
#include <glm/glm.hpp>

Game::Game() : world(50),
	     buildMode(false)
{
}

Game::~Game()
{
}

void Game::initialize()
{
  gameState = GameState::STOPPED;
  
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    fatalError("Unable to initialize SDL library");

  window = SDL_CreateWindow("My Game",
		        SDL_WINDOWPOS_CENTERED,
		        SDL_WINDOWPOS_CENTERED,
		        screenWidth,
		        screenHeight,
		        0);
  if(window == NULL)
    fatalError("Unable to open game window");
  
  renderer = SDL_CreateRenderer(window, -1, 0);
  if(renderer == NULL)
    fatalError("Unable to create renderer");  
  
  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  if(!glContext) {
    fatalError(std::string("Unable to create GL context due to ")+SDL_GetError());
  }

  glewExperimental = true;
  GLenum err = glewInit();
  if(err != GLEW_OK) {
    fatalError("Unable to initialize GLEW");
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  
  glClearColor(0.3,0.6,0.4,1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  const GLubyte* rendererVersion = glGetString(GL_VERSION);
  std::cout << "Renderer: " << rendererVersion << std::endl;

  Camera::updateMatrix();
  
  world.initialize(std::string("scenarios/world1.png"));
  
  players.push_back(new Player(&world, 0.0f, 0.0f));
  players.back()->initialize();

  float x, y;
  world.getCellCoordinates(50, 50, x, y);
  //buildings.push_back(new Building(x, y));
  //buildings.back()->initialize();

  newBuilding = new Building(&world, 0.0f, 0.0f);
  newBuilding->setAlpha(0.2f);
  newBuilding->initialize();
  
  gameState = GameState::RUNNING;
}

void Game::run()
{
  static unsigned currentTick = SDL_GetTicks();
  static unsigned lastTick;
  
  installShaders();
  
  while(gameState != GameState::STOPPED)
    {
      lastTick = SDL_GetTicks();

      processInput();
      updateGame();
      drawScene();
      updateFPS();

      currentTick = SDL_GetTicks();
      int delay = std::max(0,(int)(1000/maxFps-currentTick+lastTick));
      SDL_Delay(delay);
    }
}

void Game::processInput()
{
  // Forward events in input manager
  SDL_Event evt;
  
  while(SDL_PollEvent(&evt))
    {
      switch(evt.type)
        {
        case SDL_QUIT:
	gameState = GameState::STOPPED;
	break;

        case SDL_KEYDOWN:
	switch(evt.key.keysym.sym)
	  {
	  case SDLK_b:
	    buildMode = !buildMode;
	    std::cout << "Build mode " << (buildMode?"on":"off") << std::endl;
	  default:
	    inputManager.press(evt.key.keysym.sym);
	  }
	break;
	
        case SDL_KEYUP:
	inputManager.release(evt.key.keysym.sym);
	break;

        case SDL_MOUSEWHEEL:
	Camera::increaseZoom(0.05*evt.wheel.y);
	break;

        case SDL_MOUSEMOTION:
	inputManager.setMousePosition(evt.motion.x, evt.motion.y);
	break;
        case SDL_MOUSEBUTTONDOWN:
	if(evt.button.button == SDL_BUTTON_RIGHT)
	  {	    
	    float x, y;
	    // Compute normalized screen coordinates
	    float screenX = 2.0f*(float)(evt.button.x)/screenWidth-1.0f;
	    float screenY = 1.0f-2.0f*(float)(evt.button.y)/screenHeight;

	    // Compute world coordinates
	    Camera::getWorldFromScreenCoords(screenX, screenY, x, y);

	    // Set target player position
	    players[0]->gotoPosition(x, y, world.getShortestPath(players[0]->pos.x, players[0]->pos.y, x, y));
	  }
	break;
        default:
	break;
        }
    }

  // Process pressed keys
  //players[0]->move(inputManager.isPressed(SDLK_RIGHT), inputManager.isPressed(SDLK_LEFT),
  // inputManager.isPressed(SDLK_UP), inputManager.isPressed(SDLK_DOWN));
  /*	         
  players[0]->pos.x += 0.02f/sqrt(2.0f)*(inputManager.isPressed(SDLK_RIGHT)-inputManager.isPressed(SDLK_LEFT));
  players[0]->pos.x += 0.02f/sqrt(2.0f)*(inputManager.isPressed(SDLK_UP)-inputManager.isPressed(SDLK_DOWN));
  players[0]->pos.y += 0.02f/sqrt(2.0f)*(-inputManager.isPressed(SDLK_RIGHT)+inputManager.isPressed(SDLK_LEFT));
  players[0]->pos.y += 0.02f/sqrt(2.0f)*(inputManager.isPressed(SDLK_UP)-inputManager.isPressed(SDLK_DOWN));
  */
  
  // Scroll window
  MousePosition mousePosition = inputManager.getMousePosition();
  if(std::abs(mousePosition.x) < 10)
    Camera::scroll(-scrollSpeed, 0.0f);
  if(std::abs(screenWidth - mousePosition.x) < 10)
    Camera::scroll(scrollSpeed, 0.0f);
  if(std::abs(mousePosition.y) < 10)
    Camera::scroll(0.0f, scrollSpeed);
  if(std::abs(screenHeight - mousePosition.y) < 10)
    Camera::scroll(0.0f, -scrollSpeed);
  
  // Stop program
  if(inputManager.isPressed(SDLK_ESCAPE))
    gameState = GameState::STOPPED;
}  

void Game::installShaders()
{
  glslPrograms.push_back(GLSLProgram("data/WorldShader.vert", "data/WorldShader.frag"));
  glslPrograms.push_back(GLSLProgram("data/ObjectShader.vert", "data/ObjectShader.frag"));
  
  uniformLocationMouse = glGetUniformLocation(glslPrograms[1].getId(), "mouse");
  
  std::cout << "Prog 1: " << glslPrograms[0].getId() << std::endl;
  std::cout << "Prog 2: " << glslPrograms[1].getId() << std::endl;
}

void Game::drawScene()
{    
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw world  
  glUseProgram(glslPrograms[0].getId());

  // Set camera matrix
  GLuint cameraMatrixLocation = glGetUniformLocation(glslPrograms[0].getId(), "projection");
  glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, Camera::getCameraMatrixPtr());
  
  world.draw();  

  // Get mouse position
  MousePosition mousePosition = inputManager.getMousePosition();
  float mousePositionScreen[] = { 2.0f*(float)(mousePosition.x)/screenWidth-1.0f,
		        1.0f-2.0f*(float)(mousePosition.y)/screenHeight };

  // Draw objects
  glUseProgram(glslPrograms[1].getId());  
  
  cameraMatrixLocation = glGetUniformLocation(glslPrograms[1].getId(), "projection");
  glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, Camera::getCameraMatrixPtr());
  
  /*
    for(auto it = buildings.begin(); it != buildings.end(); ++it)
    (*it)->draw();
  */
  
  // If build mode is on
  if(buildMode)
    {
      float mousePositionWorld[2];
      Camera::getWorldFromScreenCoords(mousePositionScreen[0], mousePositionScreen[1],
			        mousePositionWorld[0], mousePositionWorld[1]);
      // std::cout << "Mouse position: " << mousePositionWorld[0] << " " << mousePositionWorld[1] << std::endl;
      newBuilding->setPosition(floor(mousePositionWorld[0])+0.5f, floor(mousePositionWorld[1])+0.5f);

      newBuilding->draw();
    }
  
  for(auto it = players.begin(); it != players.end(); ++it)
    (*it)->draw();    
  
  glUniform2fv(uniformLocationMouse, 1, mousePositionScreen);
  
  glUseProgram(0);
  SDL_GL_SwapWindow(window);
}

void Game::fatalError(const std::string& message)
{
  std::cout << "ERROR: "+message << std::endl;
  
  gameState = GameState::STOPPED;
  SDL_Quit();
}

void Game::updateFPS()
{
  static unsigned lastTick = 0;
  static unsigned currentTick = SDL_GetTicks();
  static unsigned elapsed = 0;
  static int ctr = 0;
  const int nrSamples = 100;
  
  lastTick = currentTick;
  currentTick = SDL_GetTicks();

  elapsed += currentTick - lastTick;
  ctr++;

  if(ctr == nrSamples)
    {
      fps = (float)nrSamples*1.e3/elapsed;
      elapsed = 0;
      ctr = 0;

      std::cout << "Frame rate: " << fps << " f/s" << std::endl;
    }
  
}

void Game::updateGame()
{
  for(auto player = players.begin(); player!=players.end(); ++player)
    {
      (*player)->updatePosition();
    }
}
