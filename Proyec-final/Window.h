#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise(), InitialiseFULL();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevex1() { return muevex1; }
	GLfloat getmuevex0() { return muevex0; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevez() { return muevez; }
	GLboolean getrotaP() { return rotaP; }
	GLint getcontador() { return contador; }
	GLint getactivaCamara() { return activaCamara; }
	GLint getactivaPantallaCompleta() { return activaPantallaCompleta; }
	GLint getactivaLuces() { return activaLuces; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow,*mainWindow2;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevex1;
	GLfloat muevex0;
	GLfloat muevey;
	GLfloat muevez;
	GLboolean rotaP,activaF;
	GLint contador,activaCamara,activaPantallaCompleta,activaLuces;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

