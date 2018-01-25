#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, float radius, color_t color, float speedx) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speedx = speedx;
    speedy = 0.1;
    this->radius = radius;
    this->score = floor(radius*100)-30;
    float theta = 0;
    currentSize = 0;    
    const int numPoints=120;
    GLfloat g_vertex_buffer_data[numPoints*3] = {};

    while (theta < 360) {

        GLfloat x = (GLfloat) radius * cosf(theta * M_PI / 180.0f);
        GLfloat y = (GLfloat) radius * sinf(theta * M_PI / 180.0f);
        GLfloat z = 1.1f;

        g_vertex_buffer_data[currentSize++] = x;
        g_vertex_buffer_data[currentSize++] = y;
        g_vertex_buffer_data[currentSize++] = z;
        theta = theta + (360/numPoints);
    }
    this->object = create3DObject(GL_TRIANGLE_FAN, 120, g_vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::moveLeft() {
    this->position.x -= this->speedx;
    // this->position.z = 1;
}

void Ball::pondMoveLeft(float radius) {
    float vertDisp = -2.9 - this->position.y; 
    float alpha = asin(vertDisp/radius);
    float dtheta = 0.1f;
    float angle = (alpha + dtheta/2);
    float dx = (radius*2*sinf(angle * M_PI / 180.0f)*sinf((dtheta/2) * M_PI / 180.0f));
    float dy = (radius*2*cosf(angle * M_PI / 180.0f)*sinf((dtheta/2) * M_PI / 180.0f));
    this->position.x -= dx;
    this->position.y += dy;
}

void Ball::moveRight() {
    this->position.x += this->speedx;
    // this->position.z = 1;
}

void Ball::pondMoveRight(float radius) {
    float vertDisp = -2.9 - this->position.y; 
    float alpha = asin(vertDisp/radius);
    float dtheta = 0.1f;
    float angle = (alpha + dtheta/2);
    float dx = (radius*2*sinf(angle * M_PI / 180.0f)*sinf((dtheta/2) * M_PI / 180.0f));
    float dy = (radius*2*cosf(angle * M_PI / 180.0f)*sinf((dtheta/2) * M_PI / 180.0f));
    this->position.x += dx;
    this->position.y -= dy;
}

int Ball::jump() {
    if (speedy<0){
        speedy *= -1;
        return 0;
    }
    this->position.y += speedy;
    speedy -= 0.002;
    if (speedy<=0){
        speedy *= -1;
        return 0;
    }
    return 1;
    // this->position.z = 1;
}

int Ball::fall() {
    this->position.y -= speedy;
    speedy += 0.002;
    if (this->position.y<=-2.9){
        return 0;
    }
    return 1;
    // this->position.z = 1;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, radius, radius };
    return bbox;
}
