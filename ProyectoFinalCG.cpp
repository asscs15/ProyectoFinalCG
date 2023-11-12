/*---------------------------------------------------------*/
/* ----------------  Proyecto Final             -----------*/
/*-----------------    2023-2   ---------------------------*/
/*------ Cruz Cedillo Daniel Alejandro		  -------------*/
/*------------- No. Cuenta: 316083298---------------*/
/*------ Chávez Flores Giovanni		  -------------*/
/*------------- No. Cuenta: 317053319---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <irrKlang.h> //Sonido


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 15.0f, 70.0f));//Posicion incial de la camara 
float MovementSpeed = 1000.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float range = 0.0f,
rot = 0.0f,
movCamera = 0.0f;
bool First = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f); //Direccion de la fuente de luz
glm::vec3 direccionluz(0.0f, -1.0f, 0.0f);
glm::vec3 myposition02(80.0f, 4.0f, 0.0f);
glm::vec3 myColor01(0.0f, 0.0f, 0.0f);

//Luz escenario
float luzdif = 0.55f,
luzspec = 0.2f;

//luz arcade
float luzarcspec = 0.5f,
luzarcdif = 1.0f;
bool encendidoarcade = true;

//-------------------------------------------------------------------------------------
// Daclaracion de variables que se usaran para las animaciones
// ------------------------------------------------------------------------------------
//Luz ciclo de dia y noche
float luzx = 0.5f, luzy = 0.8f, luzz = 0.8f, noche = 0.0f;
int dia = 0;

//Sonic
float posxs = 0.0f,
poszs = 0.0f,
posys = 0.0f,
rotsonic =0.0f,
incsonic = 0.0f;
int animsonic = 0;

//Freddy
float rotBrazoF = 0.0f;
int animFreddy = 0;
bool Freddyanim = true;

//Chica
float rotBrazoC = 0.0f, 
	rotpanque = 0.0f,
	poszpanque = 18.5f;
int animChica = 0;
bool Chicaanim = false;

//eggman
float eggx = 0.0f,
	eggy = 0.0f,
	eggz = 0.0f,
	rotegg = 0.0f,
	egginc =0.0f;
int animegg=0;

//Cheff
float rotcheff=0.0f,
	poszsar = 13.5f,
	rotsarten=0.0f,
	carnex =0.0f,
	carney = 0.0f,
	carnez =0.0f,
	tempcarne = 0.0f,
	carneinc =1.5f;
int animcheff = 0;

//BALLON BOY
glm::vec3 prueba(0.0f, 0.0f, 0.0f);
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
rotRodDer = 0.0f,
movPierDer = 0.0f,
movPierIzq = 0.0f,
giroMonito = 0.0f,
movBrazoDer = 0.0f, //necesito agregar variable de dibujo, este es el principio del proceso
movBrazIzq = 0.0f,
movCabeza = 0.0f,
movHombroIzq = 0.0f,
movHombroDer = 0.0f;
bool baja = false;
bool sube = true;
//Camara BB
float BBCameraX = 0.0f, BBCameraZ = 0.0f;

void movCuerpo() {
	if (movPierDer <= 40.0f && sube) {
		movPierDer += 3.0f;
		movHombroDer += 0.1f;
		movBrazoDer += 0.12f;
		movPierIzq -= 3.0f;
		rotRodDer += 1.0f;
		rotRodIzq -= 1.0f;
		movHombroDer -= 0.1f;
		movBrazIzq -= 0.1f;
		if (movPierDer >= 40.0f) {
			sube = false;
			baja = true;
		}
	}
	if (movPierDer >= -50 && baja) {
		movPierDer -= 3.0f;
		movHombroDer -= 0.1f;
		movBrazoDer -= 0.12f;
		movPierIzq += 3.0f;
		rotRodDer -= 1.0f;
		rotRodIzq += 1.0f;
		movHombroDer += 0.1f;
		movBrazIzq += 0.1f;
		if (movPierDer <= -50.0f) {
			baja = false;
			sube = true;
		}
	}
}






//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);


//--------------------------------------------------------------------------------------------
//Rings Keyframes
float rotring = 0.0f;
float poszring = 0.0f;
//Calculo incremento
float incrotring = 0.0f, 
incposz = 0.0f;

int i = 0, pos =0;
#define MAX_FRAMES 50
int i_max_steps = 60;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float poszring;
	float rotring;

}FRAME; //Estructura Frame

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 4;			//Cantidad de Frames
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	
	KeyFrame[FrameIndex].poszring = poszring;
	KeyFrame[FrameIndex].rotring = rotring;
	
	FrameIndex++;
}

//Punto de incio para reproducir
void resetElements(void)
{
	rotring = KeyFrame[0].rotring;
	poszring = KeyFrame[0].poszring;
}

void interpolation(void)
{
	incrotring= (KeyFrame[playIndex + 1].rotring - KeyFrame[playIndex].rotring) / i_max_steps;
	incposz = (KeyFrame[playIndex + 1].poszring - KeyFrame[playIndex].poszring) / i_max_steps;
}

//-----------------------------------------------------------------------
//Animaciones
//-------------------------------------------------------------------

void animate(void)
{
	//animacion para el ciclo de dia y de noche
	/* El ciclo de dia y de noche sera aproximadamente de 1 min, en le caso 0 es para 
	hacer de dia, el caso 1 es para oscurecer 	y el caso 2 no cambia la iluminacion 
	pero si mantiene los valores ´para simularla */
	switch (dia) {
		case 0: 
			luzx += 0.0005f;
			luzy += 0.0005;
			luzz += 0.0005;
			if (luzx >= 0.8f)
				dia = 1;
			break;
		case 1:
			luzx -= 0.0005f;
			luzy -= 0.0005;
			luzz -= 0.0005;
			if (luzx <= 0.05f) {
				luzdif = 0.0;
				luzspec = 0.0;
				dia = 2;
				printf("noche ");
			}
				
			break;
		case 2:
			noche += 0.001;
			if (noche >= 1.0f) {
				dia = 0;
				noche = 0.0f;
				luzdif = 0.55;
				luzspec = 0.2;
				printf("dia ");
			}
		
	}
	
	//--------------------------------------------------------------------------------
	//Animacion por keyframes
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("Frame [%d] reproducido \n", playIndex - 1);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Ultimo frame es [%d] \n", FrameIndex - 1);
				//fprintf(guardarFrames,"FrameIndex [%d]" ,FrameIndex);
				printf("Termina animaci�n\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu�\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			poszring += incposz;
			rotring += incrotring;
			i_curr_steps++;
		}
	}
	//--------------------------------------------------------------------------------
	
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// --------------------
	// glfw window creation
	// --------------------
	
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//-------------------------------------------------------------------------
	//Sonido de fondo
	//---------------------------------------------------------------------
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	
	if (!engine) {
		printf("Could not startup engine ");
		return 0; //eror inciando engine
	}

	engine->play2D("fnaf.mp3", true);
	engine->setSoundVolume(0.4f);
	//--------------------------------
	// configure global opengl state
	// ------------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/rightcity.jpg", 
		"resources/skybox/leftcity.jpg",
		"resources/skybox/topcity.jpg",
		"resources/skybox/bottomcity.jpg",
		"resources/skybox/frontcity.jpg", 
		"resources/skybox/backcity.jpg" 
	};

	Skybox skybox = Skybox(faces);


	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/Modelos/PisoEscenario/piso.obj");
	
	//--------------------------------------------------------------------------------
	//Modelos Proyecto
	//-------------------------------------------------------------------------------
	//Elementos sin animacion
	//--------------------------------------------------------------------------------
	//Museo
	Model museo("resources/Modelos/Museo/edificio.obj");
	//--------------------------------------------------------------------------------
	//Modelos con animacion
	//--------------------------------------------------------------------------------
	//Caparazon

	//Kart
	Model kartM("resources/Modelos/Mario/Mario/MarioKart.obj");
	Model kartL("resources/Modelos/Mario/Luigi/luigi_kart.obj");
	Model llantasKartM("resources/Modelos/Mario/Kart/LlantaNormal/LlantaNormal.obj");
	//llantas

	//Aku AKu

	//persona random

	//para keyframes
	animate();
	//Inicialización de KeyFrames


	////Inicialización de KeyFrames
	//for (int i = 0; i < MAX_FRAMES; i++)
	//{

	//}
	string arch = "ring.txt";
	ifstream archivo(arch.c_str());
	string lineas;


	while (getline(archivo, lineas)) {

		printf("%i ", i);
		switch (i) {
		case 0:
			KeyFrame[pos].poszring = std::stof(lineas);
			break;
		case 1:
			KeyFrame[pos].rotring = std::stof(lineas);
			break;

		}
		if (i < 2) {
			i++;
		}
		if (i == 2) {
			pos++;
			i = 0;
		}

	}

	glm::mat4 projection = glm::perspective(camera.getZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		if (!camera.getIsometric()) {
			projection = glm::perspective(camera.getZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		}
		else {
			projection = glm::ortho(-camera.getIsoZoom(), camera.getIsoZoom(), -camera.getIsoZoom(), camera.getIsoZoom(), -30.0f, 40.0f);

		}

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		//------------------------------------------------------------------------------------------------------------------------
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(luzx, luzy, luzz));//Da luz a todo
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));	//Da luz desde un únto
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));	//Brillo sobre una superficie

		//Luz lampara salon
		staticShader.setVec3("pointLight[0].position", glm::vec3(0.0, 70.0f, -150.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(luzdif, luzdif, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(luzspec, luzspec, luzspec));
		staticShader.setFloat("pointLight[0].constant", 0.008f); //Potencia de la luz
		staticShader.setFloat("pointLight[0].linear", 0.00009f); //distancia de luz, control mas fino
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);//distancia de luz, es mas brusco, a mas pequeño menor atenuacion mas viaja la luz

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 70.0f, -50.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(luzdif, luzdif, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(luzspec, luzspec, luzspec));
		staticShader.setFloat("pointLight[1].constant", 0.008f);
		staticShader.setFloat("pointLight[1].linear", 0.00009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		//Luz cocina
		staticShader.setVec3("pointLight[2].position", glm::vec3(-200.0, 70.0f, -50.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(luzdif, luzdif, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(luzspec, luzspec, luzspec));
		staticShader.setFloat("pointLight[2].constant", 0.008f);
		staticShader.setFloat("pointLight[2].linear", 0.00009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		//Luz arcade
		staticShader.setVec3("pointLight[3].position", glm::vec3(170, 20.0f, 10.0f));
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(luzarcdif, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(luzarcspec, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 0.008f);
		staticShader.setFloat("pointLight[3].linear", 0.1f);
		staticShader.setFloat("pointLight[3].quadratic", 0.0032f);

		//fuente de luz reflector
		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));//Posicion	
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));//Direccion a donde apunta la luz
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.3f, 0.3f, 0.3f));//
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));//Maxima iluminacion
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));//Disminucion de la intensidad
		staticShader.setFloat("spotLight[0].constant", 0.5f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);//Distancia que viajara la luz
		staticShader.setFloat("spotLight[0].quadratic", 0.005);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		// Restaurante
		// -------------------------------------------------------------------------------------------------------------------------

		/*model = glm::translate(model, glm::vec3(0.0f, -0.7f, -100.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		restaurante.Draw(staticShader);*/

		//Mario_Kart
		// -------------------------------------------------------------------------------------------------------------------------
		//Mario
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -170.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		kartM.Draw(staticShader);
		//Luigi
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -170.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		kartL.Draw(staticShader);
		

		//Pasto Diorama
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(model, glm::vec3(0.0f, -20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f,1.0f,100.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//Estrucutura Museo
		// ----------------------------------------------------------------------------------------------------------------------
		model = glm::translate(model, glm::vec3(0.0f, 15.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 5.0f, 0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		museo.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		
	}

	//Activcar animaciones 
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) //Activar o desactivar anmacion Freddy
		Freddyanim ^= true;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) //Activar o desactivar animacion Chica
		Chicaanim ^= true;
	/*if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		animacion ^= true;*/
		//Activar o desactivar luces
	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
		encendidoarcade ^= true;


	

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
	//BALLONBOY

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		posZ--;
		movCuerpo();
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		posX++;
		movCuerpo();
	}


	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		posZ++;
		movCuerpo();
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		posX--;
		movCuerpo();
	}

	//CABEZA
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && movCabeza < 75.0f)
		movCabeza += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && movCabeza > -75.0f)
		movCabeza -= 3.5f;

	//ROTACION
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		giroMonito--;
		movCuerpo();
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		giroMonito++;
		movCuerpo();
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		camera.setIsometric(true);
		
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		camera.setIsometric(false);
		
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		camera.setZoom(camera.getIsoZoom() + 0.5f);
		if (camera.getIsoZoom() >= 30.0f) camera.setZoom(30.0f);
		
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		camera.setZoom(camera.getIsoZoom() - 0.5f);
		if (camera.getIsoZoom() <= 2.0f) camera.setZoom(2.0f);
		
	}
	//HOMBRO DERECHO
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && movHombroDer < 2.0f)
		movHombroDer += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && movHombroDer > -0.1f)
		movHombroDer -= 3.5f;
	//HOMBRO IZQUIERDO
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && movHombroIzq < 2.0f)
		movHombroIzq += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && movHombroIzq > -0.1f)
		movHombroIzq -= 3.5f;
	//BRAZO DERECHO
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && movBrazoDer < 0.5f)
		movBrazoDer += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && movBrazoDer > 0.0f)
		movBrazoDer -= 3.5f;
	//BRAZO IZQUIERDO
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && movBrazIzq < 0.5f)
		movBrazIzq += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && movBrazIzq > 0.0f)
		movBrazIzq -= 3.5f;

	//PIERNA ARRIBA DERECHA
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && movPierDer < 70.0f)
		movPierDer += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && movPierDer > -90.0f)
		movPierDer -= 3.5f;
	//PIERNA ARRIBA IZQUIERDA
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && movPierIzq < 70.0f)
		movPierIzq += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && movPierIzq > -90.0f)
		movPierIzq -= 3.5f;

	//RODILLA DERECHA
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && rotRodDer < 70.0f)
		rotRodDer++;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && rotRodDer > 0.0f)
		rotRodDer--;

	//RODILLA IZQUIERDA
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && rotRodIzq < 70.0f)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && rotRodIzq > 0.0f)
		rotRodIzq--;

	//ROTACION
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		giroMonito--;
		movCuerpo();
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		giroMonito++;
		movCuerpo();
	}
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}