#include "Window.h"
#include"stb_image.h"


Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	muevex0 = 0.0f;
	muevex1 = 0.0f;
	muevey = 0.0f;
	muevez = 0.0f;
	rotaP = true;
	bool activaF = true;
	activaCamara = 0;
	contador = 0;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);//Para maximizar la ventana

	

	//CREAR VENTANA
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//mainWindow = glfwCreateWindow(mode->width, mode->height, "Proyecto", monitor, NULL);//Ventana completa
	mainWindow = glfwCreateWindow(width, height, "Proyecto", NULL, NULL);//Ventana maximizada
		
	//CREAR ICONO DE LA VENTANA
	GLFWimage images[1];
	images[0].pixels = stbi_load("icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(mainWindow, 1, images);
	stbi_image_free(images[0].pixels);


	if (!mainWindow){
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK){
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
								// Asignar valores de la ventana y coordenadas

								//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowUserPointer(mainWindow, this);	
	
}

int Window::InitialiseFULL(){
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);//Para maximizar la ventana


	//CREAR VENTANA
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	mainWindow = glfwCreateWindow(mode->width, mode->height, "Proyecto", monitor, NULL);//Ventana completa
	//mainWindow = glfwCreateWindow(width, height, "Proyecto", NULL, NULL);//Ventana maximizada

	//CREAR ICONO DE LA VENTANA
	GLFWimage images[1];
	images[0].pixels = stbi_load("icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(mainWindow, 1, images);
	stbi_image_free(images[0].pixels);


	if (!mainWindow) {
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);
	//MANEJAR TECLADO y MOUSE
	createCallbacks();
	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
								// Asignar valores de la ventana y coordenadas

								//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_F)
	{
		theWindow->rotaP = true;
	}

	if (key == GLFW_KEY_G)
	{
		theWindow->rotaP = false;
	}


	if (key == GLFW_KEY_Z)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow-> muevex -= 1.0;
	}

	if (key == GLFW_KEY_U)//Mover en eje x+
	{
		theWindow->muevex0 += 1.0;
	}
	if (key == GLFW_KEY_Y)//Mover en eje x-
	{
		theWindow->muevex0 -= 1.0;
	}
	if (key == GLFW_KEY_C)//Mover en eje y+
	{
		theWindow->muevey += 1.0;
	}
	if (key == GLFW_KEY_V)//Mover en eje y-
	{
		theWindow->muevey -= 1.0;
	}

	if (key == GLFW_KEY_I)//Mover en eje y+
	{
		theWindow->muevez += 1.0;
	}
	if (key == GLFW_KEY_O)//Mover en eje y-
	{
		theWindow->muevez -= 1.0;
	}

	if (key == GLFW_KEY_L)//Mover en eje y-
	{
		theWindow->contador = 0.0;
	}

	if (key == GLFW_KEY_N)//Activa camaraIsometrica
	{
		theWindow->activaCamara = 1;
		printf("\nSe activo camara Isometrica.\n");
	}
	if (key == GLFW_KEY_M)//Activa camaraNormal
	{
		theWindow->activaCamara = 0;
		printf("\nSe activo camara normal.\n");
	}

	if (key == GLFW_KEY_J)//Activa las luces
	{
		theWindow->activaLuces = 1;
		printf("\nLuces encendidas.\n");
	}
	if (key == GLFW_KEY_H)//Apaga las luces
	{
		theWindow->activaLuces = 0;
		printf("\nLuces apagadas.\n");
	}

	if (key == GLFW_KEY_Q)//Activa modo pantalla completa
	{
		theWindow->activaPantallaCompleta = 1;
	}

	if (key == GLFW_KEY_Q)//Activa modo pantalla completa
	{
		theWindow->activaPantallaCompleta = 1;
	}
	/*int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	printf("\nControl 1 status: %d\n", present);
	int axesCount;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
	if (GLFW_PRESS == buttons[0]) {
		printf("Boton A presionado\n");
		theWindow->movZero += deltaTime * 10;
	}*/


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
