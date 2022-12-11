#include "InputManager.h"

void InputManager::press(unsigned int key)
{
  keyPressed[key] = true;
}

void InputManager::release(unsigned int key)
{
  keyPressed[key] = false;
}

bool InputManager::isPressed(unsigned int key)
{
  auto it = keyPressed.find(key);
  if(it == keyPressed.end())
    return false;

  return it->second;
}

void InputManager::setMousePosition(int x, int y)
{
  mousePosition.x = x;
  mousePosition.y = y;
}

MousePosition InputManager::getMousePosition() const
{
  return mousePosition;
}
