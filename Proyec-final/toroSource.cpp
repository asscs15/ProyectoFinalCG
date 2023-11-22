// Draw rotating Torus with GLFW3
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glfw3.h>

void setCoords(double r, double c, int rSeg, int cSeg, int i, int j,
    GLfloat* vertices, GLfloat* uv) {
    const double PI = 3.1415926535897932384626433832795;
    const double TAU = 2 * PI;

    double x = (c + r * cos(i * TAU / rSeg)) * cos(j * TAU / cSeg);
    double y = (c + r * cos(i * TAU / rSeg)) * sin(j * TAU / cSeg);
    double z = r * sin(i * TAU / rSeg);

    vertices[0] = 2 * x;
    vertices[1] = 2 * y;
    vertices[2] = 2 * z;

    uv[0] = i / (double)rSeg;
    uv[1] = j / (double)cSeg;
}

int createObject(double r, double c, int rSeg, int cSeg, GLfloat** vertices,
    GLfloat** uv) {
    int count = rSeg * cSeg * 6;
    *vertices = (GLfloat*)malloc(count * 3 * sizeof(GLfloat));
    *uv = (GLfloat*)malloc(count * 2 * sizeof(GLfloat));

    for (int x = 0; x < cSeg; x++) { // through stripes
        for (int y = 0; y < rSeg; y++) { // through squares on stripe
            GLfloat* vertexPtr = *vertices + ((x * rSeg) + y) * 18;
            GLfloat* uvPtr = *uv + ((x * rSeg) + y) * 12;
            setCoords(r, c, rSeg, cSeg, x, y, vertexPtr + 0, uvPtr + 0);
            setCoords(r, c, rSeg, cSeg, x + 1, y, vertexPtr + 3, uvPtr + 2);
            setCoords(r, c, rSeg, cSeg, x, y + 1, vertexPtr + 6, uvPtr + 4);

            setCoords(r, c, rSeg, cSeg, x, y + 1, vertexPtr + 9, uvPtr + 6);
            setCoords(r, c, rSeg, cSeg, x + 1, y, vertexPtr + 12, uvPtr + 8);
            setCoords(r, c, rSeg, cSeg, x + 1, y + 1, vertexPtr + 15,
                uvPtr + 10);
        }
    }

    return count;
}

// 1rst attribute buffer : vertices
glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glVertexAttribPointer(0, // attribute. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
);

// 2nd attribute buffer : UVs
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glVertexAttribPointer(1,2,                                
    GL_FLOAT,                        
    GL_FALSE,                         
    0,                                
    (void*)0                          // array buffer offset
);

glFrontFace(GL_CW);

// Draw the triangle !
glDrawArrays(GL_TRIANGLES, 0, vertices * 3); // 12*3 indices starting at 0 -> 12 triangles

glFrontFace(GL_CCW);

glDisableVertexAttribArray(0);
glDisableVertexAttribArray(1);