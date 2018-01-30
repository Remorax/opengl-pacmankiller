#include "porcupine.h"
#include "main.h"

Porcupine::Porcupine(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->speedx = 0.01;
    static const GLfloat vertex_buffer_data[] = {
         0,  0, 0, // vertex 1
         0.2,  0.2, 0, // vertex 2
         0.4,  0, 0, // vertex 3

         0.4,  0, 0, // vertex 3
         0.6,  0.2, 0, // vertex 4
         0.8,  0, 0, // vertex 5

         0.8,  0, 0, // vertex 5
         1.0,  0.2, 0, // vertex 6
         1.2,  0, 0, // vertex 7
    };

    this->object = create3DObject(GL_TRIANGLES, 9, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::move() {
    this->position.x += this->speedx;
    if (this->position.x>=0)
        this->speedx *= -1;
    if (this->position.x<=-1.95)
        this->speedx *= -1;
}

bounding_box_t Porcupine::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.1, 0.1 };
    return bbox;
}
