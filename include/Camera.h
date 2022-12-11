#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

class Camera
{
 public:
  Camera();

  void updateMatrix();
  void increaseZoom(float);
  float getZoom();
  float* getCameraMatrix();
  void getWorldFromScreenCoords(float, float, float&, float&) const;
 private:
  
  float zoom;
  glm::mat4 rotationMatrix;
  glm::mat4 cameraMatrix;
};

#endif
