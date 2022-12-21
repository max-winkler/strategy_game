default:
	g++ -c -g -Iinclude src/Game.cpp -o src/Game.o
	g++ -c -g -Iinclude src/Player.cpp -o src/Player.o
	g++ -c -g -Iinclude src/Vertex.cpp -o src/Vertex.o
	g++ -c -g -Iinclude src/TextureManager.cpp -o src/TextureManager.o
	g++ -c -g -Iinclude src/Camera.cpp -o src/Camera.o
	g++ -c -g -Iinclude src/InputManager.cpp -o src/InputManager.o
	g++ -c -g -Iinclude src/World.cpp -o src/World.o
	g++ -c -g -Iinclude src/Sprite.cpp -o src/Sprite.o
	g++ -c -g -Iinclude src/Building.cpp -o src/Building.o
	g++ -c -g -Iinclude src/GLSLProgram.cpp -o src/GLSLProgram.o
	g++ -c -g -Iinclude src/SpriteSheet.cpp -o src/SpriteSheet.o	
	g++ -c -g -Iinclude main.cpp -o main.o
	g++ -g main.o src/Game.o src/Player.o src/Vertex.o src/TextureManager.o src/Camera.o src/InputManager.o src/World.o src/Sprite.o src/Building.o src/GLSLProgram.o src/SpriteSheet.o -lGL -lGLEW -lSDL2 -lpng -o mygame

clean:
	rm src/*.o mygame
