#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    float theta = 0;
    currentSize = 0;
    const int numPoints=60;
    GLfloat g_vertex_buffer_data[numPoints*3+36] = {};
    for (int i=0; i<6; ++i)
    {
        switch(i){
            case 0:
            case 5:
                g_vertex_buffer_data[currentSize++] = -0.3;
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
            case 4:
                g_vertex_buffer_data[currentSize++] = -0.3;
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
            case 1:
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = -0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
            case 2:
            case 3:
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
        }
    }
    while (theta < 180) {

        GLfloat x = (GLfloat) radius * cosf(theta * M_PI / 180.0f);
        GLfloat y = (GLfloat) -radius * sinf(theta * M_PI / 180.0f);
        GLfloat z = 1.0f;

        g_vertex_buffer_data[currentSize++] = x;
        g_vertex_buffer_data[currentSize++] = y;
        g_vertex_buffer_data[currentSize++] = z;
        theta = theta + (180/numPoints);
    }
    for (int i=0; i<6; ++i)
    {
        switch(i){
            case 0:
            case 5:
                g_vertex_buffer_data[currentSize++] = -0.3;
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
            case 4:
                g_vertex_buffer_data[currentSize++] = -0.3;
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
            case 1:
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = -0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
            case 2:
            case 3:
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0.3;
                g_vertex_buffer_data[currentSize++] = 0;
                break;
        }
    }
    this->object = create3DObject(GL_TRIANGLE_FAN, 72, g_vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Trampoline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Trampoline::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, radius, radius };
    return bbox;
}
