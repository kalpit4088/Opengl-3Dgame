#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    speed = 0;
    speedY = 0;
    speedZ = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static GLfloat vertex_buffer_data1[] = {
        -7*1.0f, 7*1.0f, 7*1.0f,
        -7*1.0f, 7*1.0f, -7*1.0f,
         7*1.0f, 7*1.0f, 7*1.0f,

         7*1.0f,7*1.0f,7*1.0f,
         -7*1.0f, 7*1.0f, -7*1.0f,
         7*1.0f,7*1.0f,-7*1.0f,

        -7*0.5f, 7*0.5f, 7*0.5f,
        -7*0.5f, 7*0.5f, -7*0.5f,
        7*0.5f, 7*0.5f, 7*0.5f,

        -7*0.5f, 7*0.5f, -7*0.5f,
        7*0.5f, 7*0.5f, 7*0.5f,
        7*0.5f, 7*0.5f, -7*0.5f,

        -7*1.0f,7*1.0f,7*1.0f,
        -7*1.0f,7*1.0f,-7*1.0f,
        -7*0.5f,7*0.5f,7*0.5f,

        -7*1.0f, 7*1.0f, -7*1.0f,
        -7*0.5f, 7*0.5f, -7*0.5f,
        -7*0.5f, 7*0.5f, 7*0.5f,

        -7*1.0f,7*1.0f,7*1.0f,
        7*1.0f,7*1.0f,7*1.0f,
        -7*0.5f,7*0.5f,7*0.5f,

        7*1.0f,7*1.0f,7*1.0f,
        -7*0.5f,7*0.5f,7*0.5f,
        7*0.5f,7*0.5f,7*0.5f,

        -7*1.0f,7*1.0f,-7*1.0f,
        7*1.0f,7*1.0f,-7*1.0f,
        -7*0.5f,7*0.5f,-7*0.5f,

        -7*0.5f,7*0.5f,-7*0.5f,
        7*0.5f,7*0.5f,-7*0.5f,
        7*1.0f,7*1.0f,-7*1.0f,

        7*1.0f,7*1.0f,7*1.0f,
        7*1.0f,7*1.0f,-7*1.0f,
        7*0.5f,7*0.5f,7*0.5f,

        7*0.5f,7*0.5f,7*0.5f,
        7*0.5f,7*0.5f,-7*0.5f,
        7*1.0f,7*1.0f,-7*1.0f

    };

    static GLfloat vertex_buffer_data2[] = {

      0.0f,14.0f,0.0f,
      0.7f,14.0f,0.0f,
      0.7f,7.0f,0.0f,

      0.0f,14.0f,0.0f,
      0.7f,7.0f,0.0f,
      0.0f,7.0f,0.0f

    };

    static GLfloat vertex_buffer_data3[] = {
      7*0.1f,7*2.0f,0.0f,
      7*0.1f,7*1.5f,0.0f,
      7*1.5f,7*1.75f,0.0f
    };

    static GLfloat vertex_buffer_data4[] = {
      -1.4,7*1,-3.5,
      1.4,7*1,-3.5,
      1.4,7*0.6,-3.5,

      1.4,7*0.6,-3.5,
      -1.4,7*0.6,-3.5,
      -1.4,7*1,-3.5,

      -1.4,7*1,-9.5,
      1.4,7*1,-9.5,
      1.4,7*0.6,-9.5,

      1.4,7*0.6,-9.5,
      -1.4,7*0.6,-9.5,
      -1.4,7*1,-9.5,

      -1.4,7*1,-3.5,
      1.4,7*1,-3.5,
      1.4,7*1,-9.5,

      -1.4,7*1,-3.5,
      1.4,7*1,-9.5,
      -1.4,7*1,-9.5,

      -1.4,7*0.6,-3.5,
      1.4,7*0.6,-3.5,
      1.4,7*0.6,-9.5,

      -1.4,7*0.6,-3.5,
      1.4,7*0.6,-9.5,
      -1.4,7*0.6,-9.5,

      -1.4,7*1,-3.5,
      -1.4,7*1,-9.5,
      -1.4,7*0.6,-3.5,

      -1.4,7*1,4,
      -1.4,7*0.6,-9.5,
      -1.4,7*0.6,-9.5,

      1.4,7*1,-3.5,
      1.4,7*1,-9.5,
      1.4,7*0.6,-3.5,

      1.4,7*1,-9.5,
      1.4,7*0.6,-9.5,
      1.4,7*0.6,-9.5

    };


    this->object1 = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data1, color, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data2, COLOR_GREEN, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data3, COLOR_BLACK, GL_FILL);
    this->object4 = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data4, COLOR_BLACK, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
    draw3DObject(this->object4);
}

void Ball::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Ball::tick() {
//    this->rotation += speed;
    this->position.x -= speed;
    this->position.z -= speedZ;
    this->position.y += speedY;
    speedY -= 0.5;
    if(this->position.y < 0){
      this->position.y = 0;
      this->speedY = 0;
    }
}


bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    float z = this->position.z;
    bounding_box_t bbox = { x, y, z, 14, 14,7 };
    return bbox;
}
