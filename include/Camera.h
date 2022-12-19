#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

class Camera
{
 public:
  Camera();

  static void updateMatrix();

  static void increaseZoom(float);
  static void scroll(float, float);
    
  static float getZoom();
  static float* getCameraMatrixPtr();
  static glm::mat4 getCameraMatrix();
  static void getWorldFromScreenCoords(float, float, float&, float&);

 private:
  
  static float zoom;
  static float cameraPositionX;
  static float cameraPositionY;
  
  static glm::mat4 rotationMatrix;
  static glm::mat4 cameraMatrix;
};

#endif
