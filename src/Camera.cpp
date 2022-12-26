#include <iostream>
#include <glm/gtx/transform.hpp>

#include "Camera.h"

float Camera::zoom = 1.0f;
float Camera::cameraPositionX = 0.0f;
float Camera::cameraPositionY = 0.0f;
glm::mat4 Camera::rotationMatrix;
glm::mat4 Camera::cameraMatrix;

Camera::Camera() 
{
  updateMatrix();
}

void Camera::updateMatrix()
{
  rotationMatrix = glm::mat4(1.0f);  
  rotationMatrix = glm::rotate(rotationMatrix, (float)(M_PI/3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  rotationMatrix = glm::rotate(rotationMatrix, (float)(M_PI/4.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  std::cout << "Camera position: " << cameraPositionX << " " << cameraPositionY << std::endl;
  
  cameraMatrix = glm::scale(glm::mat4(1.0f), zoom*glm::vec3(1.0f, 1.0f, 0.0f))
    * glm::translate(glm::mat4(1.0f), glm::vec3(cameraPositionX, cameraPositionY, 0.0f))
    * rotationMatrix;    
}

void Camera::getWorldFromScreenCoords(float x, float y, float& worldX, float& worldY) 
{
  x = (x/zoom-cameraPositionX);
  y = (y/zoom-cameraPositionY);
  float z = tan(M_PI/3.0f)*y;
  glm::vec4 screenVec(x, y, z, 1.0f);
  glm::vec4 worldVec = glm::transpose(rotationMatrix) * screenVec;
  
  worldX = worldVec[0];
  worldY = worldVec[1];
}

void Camera::increaseZoom(float inc)
{
  zoom = std::max(0.02f, std::min(1.0f, zoom*(1.0f+inc)));
  updateMatrix();
  std::cout << "Current zoom: " << zoom << std::endl;
}

void Camera::scroll(float x, float y)
{
  cameraPositionX -= x/zoom;
  cameraPositionY -= y/zoom;

   updateMatrix();   
}

float Camera::getZoom()
{
  return zoom;
}

float* Camera::getCameraMatrixPtr()
{
  return &(cameraMatrix[0][0]);
}

glm::mat4 Camera::getCameraMatrix()
{
  return cameraMatrix;
}
