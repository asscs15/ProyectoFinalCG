/*
PROYECTO FINAL COMPUTACION GRAFICA 
ALUMNO:CALLEJAS SANCHEZ JUAN ANTONIO
No CUENTA: 316294591
GRUPO: 06
SEMESTRE 2023-2

*/


#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <windows.h>
#include <conio.h>
//#include <modelAnim.h>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include <irrKlang/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") //Inicia con irrKlang.dll



#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include "Sphere.h"
#include "Toroide.h"
#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"




FILE *guardarFrames;
const float toRadians = 3.14159265f / 180.0f;
float movCoche,movZeroX,movZeroZ,rotarPokebola,rotaPuerta,contadorDiaNoche,contadorDiaNocheOff;
float movOffset;
float rotapato;
float giroLlanta, giroLlantaOff;
bool avanza,patoadelante,activaCamara, activaPantallaCompleta,activa_rotaPuerta1, activa_rotaPuerta2, activarDia = true;
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

//Ventanas window
Window mainWindow;
//Cámaras
Camera camera,cameraIsometrica;
//Texturas
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture paredTexture;
Texture Tagave;
//materiales
Material Material_brillante;
Material Material_opaco;
//Luz direccional
DirectionalLight mainLight;
//Luz tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
//Luz tipo spotlight
SpotLight spotLights[MAX_SPOT_LIGHTS];
//SkyBox
Skybox skybox, skyboxDia, skyboxNoche;

//------Modelos 3D-------
//Museo
//--------------------------------------------
Model Museo;

//Mario
Model Mario;
Model Luigi;

	//variables animaciones mario
	int poscm = 0;
	float cmx=0.0f, cmy=0.0f, cmrot=0.0f;
	int poscl = 0;
	float clx = 0.0f, cly = 0.0f, clrot = 0.0f;


//Crash
Model Crash;
Model Coco;
Model Cortex;
Model Aku;
Model Box_power;
Model Box_fruit;
Model Box_time;

//ModelAnim 

Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//Void musica
void musica2D();

//Void musica
void musica3D();

//Void joystick
void inputJoystick();

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects() {
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	
	GLfloat cubo_vertices[] = {
		//FRONT
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.1f,  0.1f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.1f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.1f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3

		//RIGHT
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,

		//BACK
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		//LEFT
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f, 

		//DOWN
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *piramide = new Mesh();
	piramide->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(piramide);

	Mesh *piso = new Mesh();
	piso->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(piso);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *cruz = new Mesh();
	cruz->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(cruz);

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////
bool animacion = false;

//NEW// Keyframes
float posXavion = 2.0, posYavion = 2.0, posZavion = 0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;

float posXdona = 0.0, posYdona = 0.0, posZdona = -7.0;
float posXhomero = 0.0, posYhomero = 0.0, posZhomerp = -7.0;
float	movFan_x = 0.0f, movFan_y = 0.0f, movFan_z = 0.0f;
float giroFan = 90;

#define MAX_FRAMES 50
int i_max_steps = 270;
int i_curr_steps = 9;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;
	float movFan_x;		//Variable para PosicionX
	float movFan_y;		//Variable para PosicionY
	float movFan_z;
	float movFan_xInc;		//Variable para IncrementoX
	float movFan_yInc;		//Variable para IncrementoY
	float movFan_zInc;
	float giroFan;
	float giroFanInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 46;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void){

	printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	KeyFrame[FrameIndex].movFan_x = movFan_x;
	KeyFrame[FrameIndex].movFan_y = movFan_y;
	KeyFrame[FrameIndex].movFan_z = movFan_z;
	KeyFrame[FrameIndex].giroFan = giroFan;
	FrameIndex++;
}

void resetElements(void){
	
	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion= KeyFrame[0].giroAvion;
	movFan_x = KeyFrame[0].movFan_x;
	movFan_y = KeyFrame[0].movFan_y;
	movFan_z = KeyFrame[0].movFan_z;
	giroFan = KeyFrame[0].giroFan;
}

void interpolation(void){
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
	KeyFrame[playIndex].movFan_xInc = (KeyFrame[playIndex + 1].movFan_x - KeyFrame[playIndex].movFan_x) / i_max_steps;
	KeyFrame[playIndex].movFan_yInc = (KeyFrame[playIndex + 1].movFan_y - KeyFrame[playIndex].movFan_y) / i_max_steps;
	KeyFrame[playIndex].movFan_zInc = (KeyFrame[playIndex + 1].movFan_z - KeyFrame[playIndex].movFan_z) / i_max_steps;
	KeyFrame[playIndex].giroFanInc = (KeyFrame[playIndex + 1].giroFan - KeyFrame[playIndex].giroFan) / i_max_steps;
}



void animate(void){
	//Ruta carro Mario
	switch (poscm) {
	case 0:
		cmx = 0.0;
		cmy = 0.0;
		cmrot = 0.0;
		poscm = 1;
		break;
	case 1:
		if (cmx >= -2) {
			cmx -= 0.01;
		}
		else
			poscm = 2;
		break;
	case 2:
		if (cmrot <= 80) {
			cmx -= 0.01;
			cmy += 0.005;
			cmrot += 0.6;
		}else
			poscm = 3;
		break;
	
	case 3:
		if (cmy <= 1.3) {
			cmy += 0.02;
		}
		else
			poscm = 4;
		break;
	case 4:
		if (cmrot >= 0) {
			cmx -= 0.007;
			cmy += 0.01;
			cmrot -= 0.7;
		}
		else
			poscm = 5;
		break;
	case 5:
		if (cmx >= -6) {
			cmx -= 0.02;
		}
		else
			poscm = 6;
		break;
	case 6:
		if (cmrot >= -90) {
			cmx -= 0.007;
			cmy -= 0.01;
			cmrot -= 0.7;
		}
		else
			poscm = 7;
		break;
	case 7:
		if (cmrot <= 0) {
			cmx -= 0.007;
			cmy -= 0.01;
			cmrot += 0.7;
		}
		else
			poscm = 8;
		break;
	case 8:
		if (cmx >= -8) {
			cmx -= 0.02;
		}
		else
			poscm = 9;
		break;
	case 9:
		if (cmrot <= 90) {
			cmx -= 0.01;
			cmy += 0.01;
			cmrot += 0.7;
		}
		else
			poscm = 0;
		break;
	}

	//Ruta carro Luigi
	switch (poscl) {
	case 0:
		clx = 0.0;
		cly = 0.0;
		clrot = 0.0;
		poscl = 1;
		break;
	case 1:
		if (clx >= -2) {
			clx -= 0.01;
		}
		else
			poscl = 2;
		break;
	case 2:
		if (clrot <= 80) {
			clx -= 0.01;
			cly += 0.005;
			clrot += 0.6;
		}
		else
			poscl = 3;
		break;

	case 3:
		if (cly <= 1.3) {
			cly += 0.02;
		}
		else
			poscl = 4;
		break;
	case 4:
		if (clrot >= 0) {
			clx -= 0.007;
			cly += 0.01;
			clrot -= 0.7;
		}
		else
			poscl = 5;
		break;
	case 5:
		if (clx >= -6) {
			clx -= 0.02;
		}
		else
			poscl = 6;
		break;
	case 6:
		if (clrot >= -90) {
			clx -= 0.007;
			cly -= 0.01;
			clrot -= 0.7;
		}
		else
			poscl = 7;
		break;
	case 7:
		if (clrot <= 0) {
			clx -= 0.007;
			cly -= 0.01;
			clrot += 0.7;
		}
		else
			poscl = 8;
		break;
	case 8:
		if (clx >= -8) {
			clx -= 0.02;
		}
		else
			poscl = 9;
		break;
	case 9:
		if (clrot <= 90) {
			clx -= 0.01;
			cly += 0.01;
			clrot += 0.7;
		}
		else
			poscl = 0;
		break;
	}


	guardarFrames = fopen("frames.txt", "w");
	//Movimiento del objeto
	if (play)
	{	
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("Frame [%d] reproducido \n", playIndex-1);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Ultimo frame es [%d] \n", FrameIndex-1);
				//fprintf(guardarFrames,"FrameIndex [%d]" ,FrameIndex);
				printf("\nTermina animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			movFan_x += KeyFrame[playIndex].movFan_xInc;
			movFan_y += KeyFrame[playIndex].movFan_yInc;
			movFan_z += KeyFrame[playIndex].movFan_zInc;
			giroFan += KeyFrame[playIndex].giroFanInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/



int main() {
	mainWindow = Window(1024, 720); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	

	CreateObjects();
	CreateShaders();

	/*Se definen las dos cámaras a utilizar*/
	//camera = Camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	//Camara personaje

	camera = Camera(glm::vec3(-40.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 0.0f, 5.0f, 0.5f); //(60.0f, 5.0f, 0.0f)

	cameraIsometrica = Camera(glm::vec3(20.0f, 30.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -120.0f, -30.0f, 25.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pasto.png");
	pisoTexture.LoadTextureA();
	paredTexture = Texture("Textures/pared.tga");
	paredTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//Modelos Escenario
	Museo = Model();
	Museo.LoadModel ("Models/edificio.obj");
	//Mario
	Mario = Model();
	Mario.LoadModel("Models/Mario/Mario/MarioKart.obj");
	Luigi = Model();
	Luigi.LoadModel("Models/Mario/Luigi/luigi_kart.obj");

	//Modelos Crash 
	Crash = Model();
	Crash.LoadModel("Models/Crash/CrashKart.obj");
	Coco = Model();
	Coco.LoadModel("Models/Crash/CocoKart.obj");
	Cortex = Model();
	Cortex.LoadModel("Models/Crash/CortexKart.obj");
	Aku= Model();
	Aku.LoadModel("Models/Crash/AkuAku.obj");
	Box_power = Model();
	Box_power.LoadModel("Models/Crash/caja_poder.obj");
	Box_fruit = Model();
	Box_fruit.LoadModel("Models/Crash/caja_fruta.obj");
	Box_time = Model();
	Box_time.LoadModel("Models/Crash/caja_tiempo.obj");

	
//Luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(5.0f, 5.0f, 5.0f, //Luz del dia

								0.1f, 0.3f,
								0.0f, 0.0f, -1.0f);

//Contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
	//  aInt, dInt
		5.0f, 2.0f,
	// PosX  , PosY, PosZ
		-42.0f, 6.0f, -31.0f,
	//  con  , lin , exp
		0.0f, 0.2f, 0.1f);
	pointLightCount++;

//Contador de luces tipo reflector
	unsigned int spotLightCount = 0;
	//linterna luz central
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 2.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		40.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(0.96f, 0.91f, 0.0f,
		1.0f, 2.0f,
		-10.0f, 4.0f, 30.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.2f,
		20.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(0.96f, 0.91f, 0.0f,
		1.0f, 2.0f,
		-10.0f, 4.0f, 30.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.2f,
		20.0f);
	spotLightCount++;

	spotLights[3] = SpotLight(0.96f, 0.91f, 0.0f,
		1.0f, 2.0f,
		-10.0f, 4.0f, 30.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.2f,
		20.0f);
	spotLightCount++;



/*--------------------------------SKYBOX---------------------------------*/
	//Skybox predeterminado (tarde)
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	skybox = Skybox(skyboxFaces);

	//Skybox dia
	std::vector<std::string> skyboxFacesDia;
	skyboxFacesDia.push_back("Textures/Skybox/DaylightBox_Right.tga");
	skyboxFacesDia.push_back("Textures/Skybox/DaylightBox_Left.tga");
	skyboxFacesDia.push_back("Textures/Skybox/DaylightBox_Bottom.tga");
	skyboxFacesDia.push_back("Textures/Skybox/DaylightBox_Top.tga");
	skyboxFacesDia.push_back("Textures/Skybox/DaylightBox_Front.tga");
	skyboxFacesDia.push_back("Textures/Skybox/DaylightBox_Back.tga");
	skyboxDia = Skybox(skyboxFacesDia);

	//Skybox noche
	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/Noche_Left.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche_Right.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche_Bottom.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche_Top.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche_Front.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche_Back.tga");
	skyboxNoche = Skybox(skyboxFacesNoche);
	//1 - Izquierda
	//2 - Derecha
	//3 - Abajo
	//4 - Arriba
	//5 - Delante
	//6 - Atras


/*-----------------------------------------------------------------------*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	
	movCoche = 0.0f;
	movOffset  = 1.0f;

	//movZero = 0.0f;
	avanza = 1;
	activaCamara = 1;
	giroLlanta = 0.0f;
	giroLlantaOff = 10.0f;
	//giroLlantaOffset = 10;
	// 
	/*activa_rotaPuerta1 = true;
	activa_rotaPuerta2 = false;*/
	rotaPuerta = 0.0f;
	contadorDiaNoche = 0.0f;
	contadorDiaNocheOff = 1.0f;

	/*if (activa_rotaPuerta == true) {
		rotaPuerta += mainWindow.getrotaP();
		if (rotaPuerta == 45.0f) {
			activa_rotaPuerta == false;
		}
		else {

		}
	}
	else {
		rotaPuerta = mainWindow.getrotaP()*(-1);
		activa_rotaPuerta = true;
	}*/
	
	
	//KEYFRAMES DECLARADOS INICIALES
	
	KeyFrame[0].movFan_x = 0.0f;
	KeyFrame[0].movFan_y = 0.0f;
	KeyFrame[0].giroFan = 0;


	KeyFrame[1].movFan_x = -5.0f;
	KeyFrame[1].movFan_y = 0.4f;
	KeyFrame[1].giroFan = 0;


	KeyFrame[2].movFan_x = -10.0f;
	KeyFrame[2].movFan_y = 0.2f;
	KeyFrame[2].giroFan = 0;


	KeyFrame[3].movFan_x = -15.0f;
	KeyFrame[3].movFan_y = 0.4f;
	KeyFrame[3].giroFan = 0;

	KeyFrame[4].movFan_x = -20.0f;
	KeyFrame[4].movFan_y = 0.2f;
	KeyFrame[4].giroFan = 0.0f;
	
	KeyFrame[5].movFan_x = -25.0f;
	KeyFrame[5].movFan_y = 0.4f;
	KeyFrame[5].giroFan = 0.0f;

	KeyFrame[6].movFan_x = -30.0f;
	KeyFrame[6].movFan_y = 0.2f;
	KeyFrame[6].giroFan = 0.0f;

	KeyFrame[7].movFan_x = -30.0f;
	KeyFrame[7].movFan_y = 0.4f;
	KeyFrame[7].giroFan = 90.0f;//Aqui da vuelta
	
	KeyFrame[8].movFan_x = -30.0f;
	KeyFrame[8].movFan_y = 0.2f;
	KeyFrame[8].movFan_z = -10.0f;
	KeyFrame[8].giroFan = 90.0f;
	
	KeyFrame[9].movFan_x = -30.0f;
	KeyFrame[9].movFan_y = 0.4f;
	KeyFrame[9].movFan_z = -20.0f;
	KeyFrame[9].giroFan = 90.0f;

	KeyFrame[10].movFan_x = -30.0f;
	KeyFrame[10].movFan_y = 0.2f;
	KeyFrame[10].movFan_z = -30.0f;
	KeyFrame[10].giroFan = 90.0f;

	KeyFrame[11].movFan_x = -30.0f;
	KeyFrame[11].movFan_y = 0.4f;
	KeyFrame[11].movFan_z = -30.0f;
	KeyFrame[11].giroFan = 180.0f;//Vuelve a girar

	KeyFrame[12].movFan_x = -30.0f;
	KeyFrame[12].movFan_y = 0.2f;
	KeyFrame[12].movFan_z = -30.0f;
	KeyFrame[12].giroFan = 180.0f;


	KeyFrame[13].movFan_x = -25.0f;
	KeyFrame[13].movFan_y = 0.4f;
	KeyFrame[13].movFan_z = -30.0f;
	KeyFrame[13].giroFan = 180.0f;


	KeyFrame[14].movFan_x = -20.0f;
	KeyFrame[14].movFan_y = 0.2f;
	KeyFrame[14].movFan_z = -30.0f;
	KeyFrame[14].giroFan = 180.0f;


	KeyFrame[15].movFan_x = -15.0f;
	KeyFrame[15].movFan_y = 0.4f;
	KeyFrame[15].movFan_z = -30.0f;
	KeyFrame[15].giroFan = 180.0f;

	KeyFrame[16].movFan_x = -10.0f;
	KeyFrame[16].movFan_y = 0.2f;
	KeyFrame[16].movFan_z = -30.0f;
	KeyFrame[16].giroFan = 180.0f;

	KeyFrame[17].movFan_x = -5.0f;
	KeyFrame[17].movFan_y = 0.4f;
	KeyFrame[17].movFan_z = -30.0f;
	KeyFrame[17].giroFan = 180.0f;

	KeyFrame[18].movFan_x = 0.0f;
	KeyFrame[18].movFan_y = 0.2f;
	KeyFrame[18].movFan_z = -30.0f;
	KeyFrame[18].giroFan = 180.0f;

	KeyFrame[19].movFan_x = 0.0f;
	KeyFrame[19].movFan_y = 0.4f;
	KeyFrame[19].movFan_z = -20.0f;
	KeyFrame[19].giroFan = 270.0f;//Vuelve a girar

	KeyFrame[20].movFan_x = 0.0f;
	KeyFrame[20].movFan_y = 0.2f;
	KeyFrame[20].movFan_z = -10.0f;
	KeyFrame[20].giroFan = 270.0f;

	KeyFrame[21].movFan_x = 0.0f;
	KeyFrame[21].movFan_y = 0.4f;
	KeyFrame[21].movFan_z = 0.0f;
	KeyFrame[21].giroFan = 270.0f;

	KeyFrame[22].movFan_x = 0.0f;
	KeyFrame[22].movFan_y = 0.2f;
	KeyFrame[22].movFan_z = 0.0f;
	KeyFrame[22].giroFan = 360.0f;//Ultima vuelta

	KeyFrame[23].movFan_x = -10.0f;
	KeyFrame[23].movFan_y = 0.0f;
	KeyFrame[23].movFan_z = 0.0f;
	KeyFrame[23].giroFan = 360.0f;


	Sphere sp = Sphere(0.5, 10, 10);
	sp.init();
	sp.load();


	musica2D();
	//musica3D();

/*************Loop mientras no se cierra la ventana**************/
	while (!mainWindow.getShouldClose()){

		if (mainWindow.getactivaLuces() == 1)
		{
			spotLights[1].SetFlash((glm::vec3(-20.0f, 8.0f, -28.0f)), glm::vec3(0.0f, -1.5f, 0.0f));
			spotLights[2].SetFlash((glm::vec3(13.0f, 8.0f, -28.0f)), glm::vec3(0.0f, -1.5f, 0.0f));
		}
		else {
			spotLights[1].SetFlash((glm::vec3(-20.0f, 8.0f, -28.0f)), glm::vec3(0.0f, 0.0f, 0.0f));
			spotLights[2].SetFlash((glm::vec3(13.0f, 8.0f, -28.0f)), glm::vec3(0.0f, 0.0f, 0.0f));
		}

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		
		//inputJoystick();

/*---------------------------------------------------------------------------------------------------------*/	
		////Si se pulsa la tecla Q, se activa pantalla completa, en caso contrario se pulse M, camara normal
		//if (mainWindow.getactivaPantallaCompleta() == 1) {
		//	//activaPantallaCompleta = 1;
		//	//mainWindow.InitialiseFULL();
		//	//mainWindow.swapBuffers();
		//}
		//else {
		//	//activaPantallaCompleta = 0;
		//}
/*---------------------------------------------------------------------------------------------------------*/

		//Recibir eventos del usuario
		glfwPollEvents();

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
		//skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

/**********************************FALTA ARREGLAR EL CICLO DÍA NOCHE*************************************
		if (activarDia = true) {
			if (contadorDiaNoche > -10.0f) {//Posición inicial
				contadorDiaNoche -= contadorDiaNocheOff;//Llega al 10
				skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
				activarDia = true;
			}
			else {
				activarDia = false;
			}
		}
		else {
			if (contadorDiaNoche < 10.0f) {//Posición final
				contadorDiaNoche += contadorDiaNocheOff * deltaTime * 2;//Desde el +10 hasta el -10
				skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			}
			else {
				activarDia = true;
			}
		}
********************************************************************************************************/
		
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		//// luz ligada a la cámara de tipo flash
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//Museo
		glm::mat4 museo(1.0);
		
		//Mario
		glm::mat4 mario(1.0);
		glm::mat4 luigi(1.0f);
		//Crash
		glm::mat4 model(1.0);
		glm::mat4 crash(1.0);
		glm::mat4 coco(1.0);
		glm::mat4 cortex(1.0);
		glm::mat4 aku(1.0);
		glm::mat4 poder(1.0);
		glm::mat4 fruta(1.0);
		glm::mat4 tiempo(1.0);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		//plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		/***********************AQUÍ SE REALIZA EL CAMBIO DE CÁMARA*******************************/
		if (mainWindow.getactivaCamara()) { //Al pulsar N, activa cámara isométrica
			cameraIsometrica.keyControl(mainWindow.getsKeys(), deltaTime * 5.0f);
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraIsometrica.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, cameraIsometrica.getCameraPosition().x, cameraIsometrica.getCameraPosition().y, cameraIsometrica.getCameraPosition().z);
		}
		else {//Al pulsar M, activa cámara normal
			//activaCamara = 0;
			camera.keyControl(mainWindow.getsKeys(), deltaTime * 5);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			
		}
		/**********************************************************************************************/

		//Museo
		museo = glm::mat4(1.0);

		museo = glm::translate(museo, glm::vec3(0.0f, 0.2f, 0.0f));
		museo = glm::scale(museo, glm::vec3(2.0f));

		//Museo = glm::rotate(crash, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(museo));
		Museo.RenderModel();

		//Mario en carro
		mario = glm::mat4(1.0);
		mario = glm::translate(mario, glm::vec3(-42.9f+cmy, 2.7f, 2.6f+cmx));
		mario = glm::scale(mario, glm::vec3(0.03f));
		mario = glm::rotate(mario, (180+ cmrot) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mario));
		Mario.RenderModel();

		//Luigi en carro
		luigi = glm::mat4(1.0);
		luigi = glm::translate(luigi, glm::vec3(-43.4f+cly, 2.7f, 2.6f+clx));
		luigi = glm::scale(luigi, glm::vec3(0.025f));
		luigi = glm::rotate(luigi, (180+clrot) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(luigi));
		Luigi.RenderModel();
		

		//Sala de Crash
		//----------------------------------------------------------------
		//Crash con auto
		crash = glm::mat4(1.0);
		crash = glm::translate(crash, glm::vec3(-41.0f, 0.0f, -33.0f));
		crash = glm::scale(crash, glm::vec3(0.1f, 0.1f, 0.1f));
		crash = glm::rotate(crash, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(crash));
		Crash.RenderModel();

		//Coco con auto
		coco = glm::mat4(1.0);
		coco = glm::translate(coco, glm::vec3(-41.0f, 0.0f, -38.0f));
		coco = glm::scale(coco, glm::vec3(0.1f, 0.1f, 0.1f));
		coco = glm::rotate(coco, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(coco));
		Coco.RenderModel();

		//Cortex con auto
		cortex = glm::mat4(1.0);
		cortex = glm::translate(cortex, glm::vec3(-41.0f, 0.0f, -28.0f));
		cortex = glm::scale(cortex, glm::vec3(0.1f, 0.1f, 0.1f));
		cortex = glm::rotate(cortex, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cortex));
		Cortex.RenderModel();

		//Aku Aku
		aku = glm::mat4(1.0);
		aku = glm::translate(aku, glm::vec3(0.0f, 0.0f, 0.0f));
		aku = glm::scale(aku, glm::vec3(1.0f, 1.0f, 1.0f));
		aku = glm::rotate(aku, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aku));
		Aku.RenderModel();

		//Caja de poder
		poder = glm::mat4(1.0);
		poder = glm::translate(poder, glm::vec3(-41.0f, 7.0f, -38.0f));
		poder= glm::scale(poder, glm::vec3(2.0f, 2.0f, 2.0f));
		poder = glm::rotate(poder, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(poder));
		Box_power.RenderModel();

		//Caja de fruta
		fruta = glm::mat4(1.0);
		fruta = glm::translate(fruta, glm::vec3(-41.0f, 7.0f, -28.0f));
		fruta = glm::scale(fruta, glm::vec3(2.0f, 2.0f, 2.0f));
		fruta = glm::rotate(fruta, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(fruta));
		Box_fruit.RenderModel();

		//Cajas de tiempo
		tiempo = glm::mat4(1.0);
		tiempo = glm::translate(tiempo, glm::vec3(-41.0f, 12.0f, -33.0f));
		tiempo = glm::scale(tiempo, glm::vec3(2.0f, 2.0f, 2.0f));
		tiempo = glm::rotate(tiempo, giroFan * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(tiempo));
		Box_time.RenderModel();




		glUseProgram(0);

		mainWindow.swapBuffers();
	}//Cierra ciclo while

	return 0;
}

void inputKeyframes(bool* keys){
	if (keys[GLFW_KEY_SPACE])
	{
		
		if(reproduciranimacion<1)
		{	
			if (play == false && (FrameIndex > 1))
			{
				//resetElements();
				//First Interpolation				
				interpolation();
				printf("\nLa animacion ha iniciado\n");
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
				printf("\nPresiona 0 para habilitar reproducir de nuevo la animacion'\n");
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}
	
	if (keys[GLFW_KEY_L]){
		if (guardoFrame < 1){
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P]){
		if (reinicioFrame < 1){
			guardoFrame=0;
		}
	}
	
	//Implementación de teclado para la variable de traslación en X positivo
	if (keys[GLFW_KEY_1]){
		if (ciclo < 1){
			movAvion_x += 1.0f;
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}
		
	}
	//Implementación de teclado para la variable de traslación en X negativo
	if (keys[GLFW_KEY_3]) {
		if (ciclo < 1) {
			//printf("movAvion_x(-) es: %f\n", movAvion_x);
			movAvion_x -= 1.0f;
			printf("\nmovAvion_x(-) es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}
	}

	//Implementación de teclado para la variable de traslación en Y positivo
	if (keys[GLFW_KEY_4]) {
		if (ciclo < 1) {
			//printf("movAvion_y(+) es: %f\n", movAvion_y);
			movAvion_y += 1.0f;
			printf("\nmovAvion_y(+) es: %f\n", movAvion_y);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
	//Implementación de teclado para la variable de traslación en Y negativo
	if (keys[GLFW_KEY_5]) {
		if (ciclo < 1) {
			//printf("movAvion_y(-) es: %f\n", movAvion_y);
			movAvion_y -= 1.0f;
			printf("\nmovAvion_y(-) es: %f\n", movAvion_y);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}
	}

	//Implementación de teclado para la variable GIRO DEL AVIÓN
	if (keys[GLFW_KEY_6]) {
		if (ciclo < 1) {
			//printf("movAvion_y(-) es: %f\n", movAvion_y);
			giroAvion += 10.0f;
			printf("\ngiroAvion(+) es: %f\n", giroAvion);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}
	}

	//Implementación de teclado para la variable GIRO DEL AVIÓN
	if (keys[GLFW_KEY_7]) {
		if (ciclo < 1) {
			//printf("movAvion_y(-) es: %f\n", movAvion_y);
			giroAvion -= 10.0f;
			printf("\ngiroAvion(-) es: %f\n", giroAvion);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}
	}

	if (keys[GLFW_KEY_2]){
		if(ciclo2<1)
		{
		ciclo = 0;
		}
	}



}

void inputJoystick() {
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	printf("\nControl 1 status: %d\n", present);

	int axesCount;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	if (GLFW_PRESS == axes[0]) {
		printf("Left Stick X Axis\n");
	}
	else if (GLFW_PRESS == axes[1]) {
		printf("Left Stick Y Axis\n");
	}
	else if (GLFW_PRESS == axes[2]) {
		printf("Right Stick X Axis\n");
	}
	else if (GLFW_PRESS == axes[3]) {
		printf("Right Stick Y Axis\n");
	}
	else if (GLFW_PRESS == axes[4]) {
		printf("Left Trigger/LT\n");
	}
	else if (GLFW_PRESS == axes[5]) {
		printf("Right Trigger/RT\n");
	}

	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
	if (GLFW_PRESS == buttons[0]) {
		printf("Boton A presionado\n");
	}
	else if (GLFW_PRESS == buttons[1]) {
		printf("Boton B presionado\n");
	}
	else if (GLFW_PRESS == buttons[2]) {
		printf("Boton X presionado\n");
	}
	else if (GLFW_PRESS == buttons[3]) {
		printf("Boton Y presionado\n");
	}
	else if (GLFW_PRESS == buttons[4]) {
		printf("Boton LB presionado\n");
	}
	else if (GLFW_PRESS == buttons[5]) {
		printf("Boton RB presionado\n");
	}
	else if (GLFW_PRESS == buttons[6]) {
		printf("Boton SELECT presionado\n");
	}
	else if (GLFW_PRESS == buttons[7]) {
		printf("Boton START presionado\n");
	}
	else if (GLFW_PRESS == buttons[8]) {
		printf("Boton L3 presionado\n");
	}
	else if (GLFW_PRESS == buttons[9]) {
		printf("Boton R3 presionado\n");
	}
	else if (GLFW_PRESS == buttons[10]) {
		printf("Boton D-PAD UP presionado\n");
		movZeroZ += deltaTime * 10;
	}
	else if (GLFW_PRESS == buttons[11]) {
		printf("Boton D-PAD RIGHT presionado\n");
		movZeroX += deltaTime * 10;
	}
	else if (GLFW_PRESS == buttons[12]) {
		printf("Boton D-PAD DOWN presionado\n");
		movZeroZ -= deltaTime * 10;
	}
	else if (GLFW_PRESS == buttons[13]) {
		printf("Boton D-PAD LEFT presionado\n");
		movZeroX -= deltaTime * 10;
	}
	else if (GLFW_RELEASE == buttons[0]) {
		//printf("boton suelto");
	}

}


void musica2D() {
	//Inicia el módulo con los parámetros por defecto
	ISoundEngine* engine = createIrrKlangDevice();
	ISoundEngine* engine2 = createIrrKlangDevice();
	
	if (!engine){
		printf("No se pudo iniciar\n"); 
	}

	engine->play2D("media/fondo.mp3", false);//Se reproduce audio de fondo
	//engine2->play2D("media/ambiental.mp3", true);//Se reproduce audio ambiental

	//engine->drop(); 

}

