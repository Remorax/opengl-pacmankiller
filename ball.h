#include "main.h"

#ifndef Ball_H
#define Ball_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, float radius, color_t color, float speedx);
    glm::vec3 position;
    float rotation, radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void moveLeft();
    void moveRight();
    int jump();
    int fall();
    double speedx, speedy;
    int currentSize, score;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
