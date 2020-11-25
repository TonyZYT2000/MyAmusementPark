#include "Transform.h"

Transform::Transform(const glm::mat4& transMatrix, const int updateMode) :
      transform(transMatrix), mode(updateMode) {}

Transform::~Transform() {
      for (auto child : children) {
            delete child;
      }
}

void Transform::draw(const glm::mat4& C, GLuint shader) {
      for (auto child : children) {
            child->draw(C * transform, shader);
      }
}

void Transform::update() {
      // type 1 movement, up and down
      if (mode == 1) {
            if (moveFlag) {
                  transform = glm::translate(glm::vec3(0, 0.1, 0)) * transform;
                  ++counter;
                  if (counter > 100) {
                        moveFlag = !moveFlag;
                  }
            }
            else {
                  transform = glm::translate(glm::vec3(0, -0.1, 0)) * transform;
                  --counter;
                  if (counter < 0) {
                        moveFlag = !moveFlag;
                  }
            }
      }
      // type 2 movement, rotate counterclockwisely
      else if (mode == 2) {
            transform = glm::rotate(glm::radians(0.5f), glm::vec3(0, 1, 0)) * transform;
      } 
      // type 3 movement, rotate clockwisely
      else if (mode == 3) {
            transform = glm::rotate(glm::radians(-1.0f), glm::vec3(0, 1, 0)) * transform;
      }
      // type 4 movement, rotate and move up and down
      else if (mode == 4) {
            if (!moveFlag) {
                  transform = glm::translate(glm::vec3(0, 0.3, 0)) * transform;
                  ++counter;
                  if (counter > 0) {
                        moveFlag = !moveFlag;
                  }
            }
            else {
                  transform = glm::translate(glm::vec3(0, -0.3, 0)) * transform;
                  --counter;
                  if (counter < -80) {
                        moveFlag = !moveFlag;
                  }
            }
            transform = glm::rotate(glm::radians(0.5f), glm::vec3(0, 1, 0)) * transform;
      }

      for (auto child : children) {
            child->update();
      }
}

void Transform::addChild(Node* child) {
      children.push_back(child);
}

void Transform::toggleMove() {
      int temp = mode;
      mode = modeMem;
      modeMem = temp;
}