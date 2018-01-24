#include "main.h"

#ifndef Ground_H
#define Ground_H


class Ground {
public:
    Ground() {}
    Ground(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Ground_H
