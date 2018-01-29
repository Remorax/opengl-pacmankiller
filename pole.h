#include "main.h"

#ifndef Pole_H
#define Pole_H


class Pole {
public:
    Pole() {}
    Pole(float x, float y, color_t color, GLfloat* vertex_buffer_data);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Pole_H
