#include "main.h"

#ifndef Porcupine_H
#define Porcupine_H


class Porcupine {
public:
    Porcupine() {}
    Porcupine(float x, float y, color_t color);
    glm::vec3 position;
    float rotation, speedx;
    void draw(glm::mat4 VP);
    void move();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Porcupine_H
