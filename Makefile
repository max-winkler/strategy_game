default:
	g++ -c -Iinclude src/Game.cpp -o src/Game.o
	g++ -c -Iinclude src/Player.cpp -o src/Player.o
	g++ -c -Iinclude src/Vertex.cpp -o src/Vertex.o
	g++ -c -Iinclude src/TextureManager.cpp -o src/TextureManager.o
	g++ -c -Iinclude src/Camera.cpp -o src/Camera.o
	g++ -c -Iinclude src/InputManager.cpp -o src/InputManager.o
	g++ -c -Iinclude src/World.cpp -o src/World.o
	g++ -c -Iinclude src/Sprite.cpp -o src/Sprite.o
	g++ -c -Iinclude src/Building.cpp -o src/Building.o
	g++ -c -Iinclude src/GLSLProgram.cpp -o src/GLSLProgram.o	
	g++ -c -Iinclude main.cpp -o main.o
	g++ main.o src/Game.o src/Player.o src/Vertex.o src/TextureManager.o src/Camera.o src/InputManager.o src/World.o src/Sprite.o src/Building.o src/GLSLProgram.o -lGL -lGLEW -lSDL2 -lpng -o mygame

clean:
	rm src/*.o mygame
