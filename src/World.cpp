#include <iostream>
#include <random>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

#include "World.h"

World::World(unsigned x) :
  worldSize(2*x*cellWidth),
  numGridLines(2*x+1)
{
}

World::~World()
{
}

void World::initialize(const std::string& scenarioFile)
{  
  // Read map file
  unsigned mapHeight, mapWidth;
  unsigned char* mapDataPtr; // = reinterpret_cast<unsigned char*>(mapData);
  TextureManager::loadTexture(scenarioFile, mapDataPtr, mapHeight, mapWidth);

  // Store map data in a struct
  struct RGBAColor
  {
    unsigned char r, g, b, a;

    bool operator==(const RGBAColor& that) const
    {
      return (this->r == that.r &&
	    this->g == that.g &&
	    this->b == that.b &&
	    this->a == that.a);
    }    
  } *mapData = reinterpret_cast<RGBAColor*>(mapDataPtr);  

  std::cout << "Map data read\n";
  
  struct TileIndexRange
  {
    unsigned row;
    unsigned colMin, colMax;
  };

  const unsigned tilesPerRow = 16;
  struct RGBAColorHash
  {
    unsigned operator()(const RGBAColor& color) const
    {
      return 255*255*255*(unsigned)(color.r)
        + 255*255*(unsigned)(color.g)
        + 255*(unsigned)(color.b)
        + (unsigned)(color.a);
    }
  };

  std::cout << "Hash class created\n";
  
  std::unordered_map<RGBAColor, TileIndexRange, RGBAColorHash> mapTileIndexRanges
    = {{{0, 0, 255, 255}, {4, 8, 13}},
       {{0, 255, 0, 255}, {0, 0, 15}},
       {{255, 255, 0, 255}, {7, 0, 6}}};    

  std::cout << "Map tile index ranges initialized.\n";
  
  if(mapHeight != mapWidth)
    {
      std::cout << "Error: Only square shaped maps allowed\n";
      return;
    }
  numGridLines = mapHeight+1;  
  
  spriteSheet.initialize("images/tilesheet.png", 108, 108);
  // numGridLines = worldSize/cellWidth+1; 
  
  // Initialize random number generator
  std::mt19937 gen;
  std::binomial_distribution<int> idist(15, 0.2);

  const float overlap = 1.0f/50;
    
  vertices = new Vertex[2*3*numGridLines*numGridLines];
  for(int i=0; i<mapHeight; ++i)
    {
      float x = (float)i-(float)(numGridLines-1)/2.0f;
      for(int j=0; j<mapWidth; ++j)
        {
	float y = (float)j-(float)(numGridLines-1)/2.0f;
	
	// Set vertex positions in sprite
	vertices[6*(numGridLines*i+j) + 0].setPosition(x+0.0f, y+0.0f);
	vertices[6*(numGridLines*i+j) + 1].setPosition(x+1.0f+overlap, y+0.0f);
	vertices[6*(numGridLines*i+j) + 2].setPosition(x+1.0f+overlap, y+1.0f+overlap);
	vertices[6*(numGridLines*i+j) + 3].setPosition(x+0.0f, y+0.0f);
	vertices[6*(numGridLines*i+j) + 4].setPosition(x+1.0f+overlap, y+1.0f+overlap);
	vertices[6*(numGridLines*i+j) + 5].setPosition(x+0.0f, y+1.0f+overlap);
	
	// Set UV coordinates in sprite
	RGBAColor mapPixel = mapData[i*mapWidth + j];
	if(mapTileIndexRanges.find(mapPixel) == mapTileIndexRanges.end())
	  {
	    std::cout << "Error: unable to find texture index range\n";
	    std::cout << "  map color      : RGBA("
		    << (int)mapPixel.r << " " << (int)mapPixel.g
		    << " " << (int)mapPixel.b << " " << (int)mapPixel.a << ")" << std::endl;
	    std::cout << "  pixel position : " << i << " " << j << std::endl;
	      return;
	  }
	
	TileIndexRange mapTileIndexRange = mapTileIndexRanges[mapPixel];
	
	int textureIdx = mapTileIndexRange.row*tilesPerRow + mapTileIndexRange.colMin + idist(gen)%(mapTileIndexRange.colMax-mapTileIndexRange.colMin);
	glm::vec4 uv = spriteSheet.getUVs(textureIdx);
	
	vertices[6*(numGridLines*i+j) + 0].setUV(0.5*(uv.x+uv.z), uv.w);
	vertices[6*(numGridLines*i+j) + 1].setUV(uv.z, 0.25*uv.y + 0.75*uv.w);
	vertices[6*(numGridLines*i+j) + 2].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	vertices[6*(numGridLines*i+j) + 3].setUV(0.5*(uv.x+uv.z), uv.w);
	vertices[6*(numGridLines*i+j) + 4].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	vertices[6*(numGridLines*i+j) + 5].setUV(uv.x, 0.25*uv.y + 0.75*uv.w);
        }
    }
  
  
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6*numGridLines*numGridLines*sizeof(Vertex), &(vertices[0].position.x), GL_DYNAMIC_DRAW);
  
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));
  		    
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  // delete[] gridLines;      
}

void World::draw()
{
  glBindVertexArray(vao);
  // glDrawArrays(GL_LINES, 0, 8*numGridLines);

  glBindTexture(GL_TEXTURE_2D, spriteSheet.getTextureID());
  glDrawArrays(GL_TRIANGLES, 0, 6*numGridLines*numGridLines); // Starting from vertex 0; 6 vertices total -> 2 triangle  
  glBindVertexArray(0);
}

void World::getCellCoordinates(unsigned i, unsigned j, float& x, float& y) const  
{
  x = ((float)(i+0.5f)-(float)(numGridLines-1)/2);
  y = ((float)(j+0.5f)-(float)(numGridLines-1)/2);
}
