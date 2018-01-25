#include "main.h"

#ifndef Trampoline_H
#define Trampoline_H


class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y, float radius, color_t color);
    glm::vec3 position;
    float rotation, radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    int currentSize;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // POOL_H
