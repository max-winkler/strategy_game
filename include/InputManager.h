#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <unordered_map>

struct MousePosition {
  int x, y;
};
    
class InputManager
{

 public:
  void press(unsigned int);
  void release(unsigned int);
  bool isPressed(unsigned int);
  void setMousePosition(int, int);  
  MousePosition getMousePosition() const;
 private:
  
  std::unordered_map<unsigned int, bool> keyPressed;
  MousePosition mousePosition;
};

#endif
