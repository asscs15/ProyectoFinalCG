#include "Toroide.h"


Toroide::Toroide(float ratio_int, float ratio_ext):
	ratio_int(ratio_int), ratio_ext(ratio_ext), VAO(0), VBO(0), EBO(0) {
}

Toroide::~Toroide() {

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

}



void Toroide::init() {
		int i, j, k;
		float s, t, x, y, z, twopi;

		twopi = 2 * M_PI;
		for (i = 0; i < ratio_int; i++) {
			for (j = 0; j <= ratio_ext; j++) {
				for (k = 1; k >= 0; k--) {
					s = (i + k) / ratio_int + 0.5;
					t = j / ratio_ext;

					x = (1 + .1 * cos(s * twopi / ratio_int)) * cos(t * twopi / ratio_ext);
					y = (1 + .1 * cos(s * twopi / ratio_int)) * sin(t * twopi / ratio_ext);
					z = .1 * sin(s * twopi / ratio_int);
					vertexC[i * (1) + j].position = glm::vec3(x, y, z) * ratio_ext * ratio_int;
				}
			}

		}
}

void Toroide::load() {

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

}

	void Toroide::render() {
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, vertices * 3); // 12*3 indices starting at 0 -> 12 triangles
		/*glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT,(GLvoid*)(sizeof(GLuint) * 0));*/
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

	}

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
