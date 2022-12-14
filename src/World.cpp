#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <set>
#include <vector>

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
  unsigned char* mapDataPtr;
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
    TileType tileType;
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
    = {{{0, 0, 255, 255}, {4, 8, 13, TileType::WATER}},
       {{0, 255, 0, 255}, {0, 0, 15, TileType::GRASS}},
       {{255, 255, 0, 255}, {9, 1, 4, TileType::DESSERT}}};    

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
  mapTiles = new TileType[mapWidth*mapHeight];
		       
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

	mapTiles[i*mapWidth + j] = mapTileIndexRange.tileType;
	
	int textureIdx = mapTileIndexRange.row*tilesPerRow + mapTileIndexRange.colMin
	  + idist(gen)%(mapTileIndexRange.colMax-mapTileIndexRange.colMin);
	glm::vec4 uv = spriteSheet.getUVs(textureIdx);
	
	vertices[6*(numGridLines*i+j) + 0].setUV(0.5*(uv.x+uv.z), uv.w);
	vertices[6*(numGridLines*i+j) + 1].setUV(uv.z, 0.25*uv.y + 0.75*uv.w);
	vertices[6*(numGridLines*i+j) + 2].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	vertices[6*(numGridLines*i+j) + 3].setUV(0.5*(uv.x+uv.z), uv.w);
	vertices[6*(numGridLines*i+j) + 4].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	vertices[6*(numGridLines*i+j) + 5].setUV(uv.x, 0.25*uv.y + 0.75*uv.w);
        }
    }

  // Set transition tiles near water
  for(int i=0; i<mapHeight; ++i)
    {
      for(int j=0; j<mapWidth; ++j)
        {		
	if(mapTiles[i*mapWidth+j] != TileType::WATER)
	  continue;

	const int eastTileType      = 1<<0;
	const int northTileType     = 1<<1;
	const int westTileType      = 1<<2;
	const int southTileType     = 1<<3;
	const int northEastTileType = 1<<4;
	const int northWestTileType = 1<<5;
	const int southWestTileType = 1<<6;
	const int southEastTileType = 1<<7;

	// Determine number
	int neighTypes = 0;

	if(j>0 && mapTiles[i*mapWidth+j-1] != TileType::WATER)
	    neighTypes = neighTypes | westTileType;        
	if(j<mapWidth-1 && mapTiles[i*mapWidth+j+1] != TileType::WATER)
	  neighTypes = neighTypes | eastTileType;
	if(i<mapHeight-1 && mapTiles[(i+1)*mapWidth+j] != TileType::WATER)
	  neighTypes = neighTypes | northTileType;
	if(i>0 && mapTiles[(i-1)*mapWidth+j] != TileType::WATER)
	  neighTypes = neighTypes | southTileType;
	
	if(j<mapWidth-1 && i<mapHeight-1 && mapTiles[(i+1)*mapWidth+j+1] != TileType::WATER)
	  neighTypes = neighTypes | northEastTileType;
	if(j>0 && i<mapHeight-1 && mapTiles[(i+1)*mapWidth+j-1] != TileType::WATER)
	  neighTypes = neighTypes | northWestTileType;
	if(j>0 && i>0 && mapTiles[(i-1)*mapWidth+j-1] != TileType::WATER)
	  neighTypes = neighTypes | southWestTileType;
	if(j<mapWidth-1 && i>0 && mapTiles[(i-1)*mapWidth+j+1] != TileType::WATER)
	  neighTypes = neighTypes | southEastTileType;

	// Select texture depending on neighbor tiles
	int textureIdx;
	bool flip;
	
	switch(neighTypes)	  
	  {
	  case eastTileType | southEastTileType | northEastTileType:
	  case eastTileType | northEastTileType:
	  case eastTileType | southEastTileType:
	    textureIdx = 0;
	    break;
	  case northTileType | northEastTileType | northWestTileType:
	  case northTileType | northEastTileType:
	  case northTileType | northWestTileType:
	    textureIdx = 2;
	    break;
	  case southTileType | southEastTileType | southWestTileType:
	  case southTileType | southEastTileType:
	  case southTileType | southWestTileType:
	    textureIdx = 6;
	    break;
	  case westTileType | southWestTileType | northWestTileType:
	  case westTileType | northWestTileType:
	  case westTileType | southWestTileType:
	    textureIdx = 9;
	    break;
	  case southWestTileType:
	    textureIdx = 12;
	    break;
	  case southEastTileType:
	    textureIdx = 13;
	    break;
	  case northEastTileType:
	    textureIdx = 14;
	    break;
	  case northWestTileType:
	    textureIdx = 15;
	    break;	    
	  case northTileType | eastTileType | northEastTileType | southEastTileType:
	  case northTileType | eastTileType | northEastTileType | northWestTileType:
	  case northTileType | eastTileType | northEastTileType:
	    textureIdx = 1;
	    break;
	  case westTileType | southTileType | southWestTileType | northWestTileType:
	  case westTileType | southTileType | southWestTileType | southEastTileType:
	  case westTileType | southTileType | southWestTileType:
	    textureIdx = 4;
	    break;
	  case eastTileType | southTileType | southEastTileType | northEastTileType:
	  case eastTileType | southTileType | southEastTileType | southWestTileType:
	  case eastTileType | southTileType | southEastTileType:
	    textureIdx = 7;
	    break;
	  case westTileType | northTileType | northWestTileType | northEastTileType:
	  case westTileType | northTileType | northWestTileType | southWestTileType:
	  case westTileType | northTileType | northWestTileType:
	    textureIdx = 10;
	    break;	    	    
	  default:
	    neighTypes = 0;
	    break;
	  }
	
	if(neighTypes != 0)
	  {
	    glm::vec4 uv = spriteSheet.getUVs(5*tilesPerRow+textureIdx);
	    
	    vertices[6*(numGridLines*i+j) + 0].setUV(0.5*(uv.x+uv.z), uv.w);
	    vertices[6*(numGridLines*i+j) + 1].setUV(uv.z, 0.25*uv.y + 0.75*uv.w);
	    vertices[6*(numGridLines*i+j) + 2].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	    vertices[6*(numGridLines*i+j) + 3].setUV(0.5*(uv.x+uv.z), uv.w);
	    vertices[6*(numGridLines*i+j) + 4].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	    vertices[6*(numGridLines*i+j) + 5].setUV(uv.x, 0.25*uv.y + 0.75*uv.w);
	  }
        }
    }

  // Create vertex array and vertex buffer objects
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  // Buffer data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6*numGridLines*numGridLines*sizeof(Vertex), &(vertices[0].position.x), GL_DYNAMIC_DRAW);

  // Enable attribute pointers
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // Define attributes
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));

  // Unbind
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

std::pair<float,float> World::getCellMidpoint(unsigned i, unsigned j) const
{
  return std::make_pair(((float)i+0.5f)/4.0f-(float)(numGridLines-1)/2,
		    ((float)j+0.5f)/4.0f-(float)(numGridLines-1)/2);
}

std::pair<unsigned,unsigned> World::getCellIndices(float x, float y) const
{
  return std::make_pair((unsigned)(4*(x+(numGridLines-1)/2.0f)),
		    (unsigned)(4*(y+(numGridLines-1)/2.0f)));
}

std::vector<std::pair<unsigned, unsigned>> World::getShortestPath(float startX, float startY,
						      float targetX, float targetY) const
{
  unsigned startI = (unsigned)(4*(startX+(numGridLines-1)/2.0f));
  unsigned startJ = (unsigned)(4*(startY+(numGridLines-1)/2.0f));
  unsigned targetI = (unsigned)(4*(targetX+(numGridLines-1)/2.0f));
  unsigned targetJ = (unsigned)(4*(targetY+(numGridLines-1)/2.0f));

  std::cout << "Going from position (" << startI << "," << startJ << ") to (" << targetI << "," << targetJ << ")\n";
  
  struct Cell
  {
    Cell() {}
    Cell(unsigned i, unsigned j, float gScore, float hScore)
      : i(i), j(j), gScore(gScore), hScore(hScore), fScore(gScore+hScore) {}
    
    bool operator<(const Cell& other) const
    {
      return this->fScore < other.fScore;
    }
    
    unsigned i, j;
    float gScore, hScore, fScore;
  };

  class FindByIdx
  {
  public:
    FindByIdx(unsigned i, unsigned j) : i(i), j(j) {}
    bool operator()(const Cell& cell)
    {
      return (cell.i == i && cell.j == j);
    }
  private:
    unsigned i, j;
  };
  
  typedef std::map<std::pair<unsigned,unsigned>, Cell> List;

  // Manhatten distance function
  auto distance = [](unsigned startI, unsigned startJ, unsigned targetI, unsigned targetJ) ->float
  {
    return (float)(std::abs((int)(startI - targetI)) + std::abs((int)(startJ - targetJ)));
  };
  
  List openList, closedList;
  std::map<std::pair<unsigned, unsigned>, std::pair<unsigned,unsigned>> cameFrom;

  // Insert start element
  float hScore = distance(startI, startJ, targetI, targetJ);
  openList[std::make_pair(startI,startJ)] =  Cell(startI, startJ, 0, hScore);

  while(!openList.empty())
    {
      // Extract element with smallest fScore

      // Find smallest element      
      List::iterator smallestPtr = openList.begin();
      for(List::iterator currentPtr = openList.begin(); currentPtr != openList.end(); ++currentPtr)
        if(currentPtr->second.fScore < smallestPtr->second.fScore)
	smallestPtr = currentPtr;
      
      Cell current = smallestPtr->second;
      openList.erase(smallestPtr);
      std::pair<unsigned, unsigned> currentIdxPair(current.i, current.j);
      closedList[currentIdxPair] = current;
      
      if(current.i == targetI && current.j == targetJ)
        break;
      
      // Create list of neighbors
      std::vector<Cell> neighbors = {
        Cell(current.i+1, current.j,   current.gScore+1.0f,        distance(current.i+1, current.j,   targetI, targetJ)),
        Cell(current.i+1, current.j+1, current.gScore+sqrtf(2.0f), distance(current.i+1, current.j+1, targetI, targetJ)),
        Cell(current.i,   current.j+1, current.gScore+1.0f,        distance(current.i, current.j+1,   targetI, targetJ)),
        Cell(current.i-1, current.j+1, current.gScore+sqrtf(2.0f), distance(current.i-1, current.j+1, targetI, targetJ)),
        Cell(current.i-1, current.j,   current.gScore+1.0f,        distance(current.i-1, current.j,   targetI, targetJ)), 
        Cell(current.i-1, current.j-1, current.gScore+sqrtf(2.0f), distance(current.i-1, current.j-1, targetI, targetJ)),
        Cell(current.i,   current.j-1, current.gScore+1.0f,        distance(current.i, current.j-1,   targetI, targetJ)),
        Cell(current.i+1, current.j-1, current.gScore+sqrtf(2.0f), distance(current.i+1, current.j-1, targetI, targetJ))
      };

      for(auto it=neighbors.begin(); it != neighbors.end(); ++it)
        {
	// Check if cell exceeds boundary
	if(it->i < 0 || it->i >= 4*(numGridLines-1) || it->j < 0 || it->j >= 4*(numGridLines-1))
	  continue;

	// Check if cell is a water tile
	if(mapTiles[(it->i)/4*(numGridLines-1) + (it->j)/4] == TileType::WATER)
	  continue;

	std::pair<unsigned,unsigned> idxPair(it->i, it->j);
	// Find tile in openList
	List::iterator elem_it = openList.find(idxPair);
	if(elem_it == openList.end())
	  {
	    // Check if element is in closed list
	    List::iterator closed_elem_it = closedList.find(idxPair);
	    if(closed_elem_it != closedList.end())
	      {
	        // If element is in closed list, shift to open list when new path is shorter
	        if(closed_elem_it->second.gScore > it->gScore)
		{
		  closedList.erase(closed_elem_it);
		  openList[idxPair] = *it;
		  cameFrom[idxPair] = currentIdxPair;
		}
	      }
	    else
	      {
	        // Tile is neither in open nor in closed list
	        openList[idxPair] = *it;
	        cameFrom[idxPair] = currentIdxPair;
	      }	    	    
	  }
	else
	  {
	    // Element is in open list
	    if(elem_it->second.gScore > it->gScore)
	      {
	        // The new path to the neighbor is better than the previously found one
	        openList[idxPair] = *it;
	        cameFrom[idxPair] = currentIdxPair;
	      }
	  }
        }
    }

  // Build up Route
  std::vector<std::pair<unsigned, unsigned>> route;
  std::pair<unsigned, unsigned> parent(targetI, targetJ);
  route.push_back(parent);
  std::cout << "Found route: ";
  std::cout << "(" << parent.first << "," << parent.second << ") ";

  int k=0;
  while(k<100 && parent != std::pair<unsigned, unsigned>(startI, startJ))
    {
      parent = cameFrom[parent];
      route.push_back(parent);
      std::cout << "(" << parent.first << "," << parent.second << ") ";
      k++;
    }
  std::cout << std::endl;
  if(k==100)
    std::cout << "Some error occured.\n";
  
  return route;
}

