#include <iostream>
#include <glm/gtx/transform.hpp>

#include "Camera.h"

Camera::Camera() : zoom(1.0f), cameraMatrix(1.0f)
{
  updateMatrix();
}

void Camera::updateMatrix()
{
  rotationMatrix = glm::mat4(1.0f);  

  // This worked quite well  
  rotationMatrix = glm::rotate(rotationMatrix, (float)(M_PI/3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  rotationMatrix = glm::rotate(rotationMatrix, (float)(M_PI/4.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  cameraMatrix = glm::scale(glm::mat4(1.0f), zoom*glm::vec3(1.0f, 1.0f, 0.0f)) * rotationMatrix;
    
}

void Camera::getWorldFromScreenCoords(float x, float y, float& worldX, float& worldY) const
{
  float z = tan(M_PI/3.0f)*y;
  glm::vec4 screenVec(x, y, z, 1.0f);
  glm::vec4 worldVec = glm::scale(glm::mat4(1.0f), 1.0f/zoom*glm::vec3(1.0f, 1.0f, 0.0f))*glm::inverse(rotationMatrix)*screenVec;
  
  worldX = worldVec[0];
  worldY = worldVec[1];
}

void Camera::increaseZoom(float inc)
{
  std::cout << "Increasing zoom by " << inc << std::endl;
  zoom = std::max(0.02f, std::min(1.0f, zoom*(1.0f+inc)));
  updateMatrix();
  std::cout << "Current zoom: " << zoom << std::endl;
}

float Camera::getZoom()
{
  return zoom;
}

float* Camera::getCameraMatrix()
{
  return &(cameraMatrix[0][0]);
}
