#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cstdlib>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, screenWidth, screenHeight); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment






    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render OpenGL here




        int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
        //std::cout << present << std::endl;


        if (1 == present) {
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
        
            /*
            printf("Left Stick X Axis: %f\n",axes[0]);// tested with PS4 controller connected via micro USB cable
            printf("Left Stick Y Axis: %f\n",axes[1]);// tested with PS4 controller connected via micro USB cable
            printf("Right Stick X Axis: %f\n",axes[2]);// tested with PS4 controller connected via micro USB cable
            printf("Right Stick Y Axis: %f\n",axes[3]);// tested with PS4 controller connected via micro USB cable
            printf("Left Trigger/L2: %f\n",axes[4]);// tested with PS4 controller connected via micro USB cable
            printf("Right Trigger/R2: %f\n",axes[5]);// tested with PS4 controller connected via micro USB cable
            */
            



            int buttonCount;
            const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
            if (GLFW_PRESS == buttons[0]){
                printf("Boton A presionado\n");
            }else if (GLFW_PRESS == buttons[1]) {
                printf("Boton B presionado\n");
            }else if (GLFW_PRESS == buttons[2]) {
                printf("Boton X presionado\n");
            }else if (GLFW_PRESS == buttons[3]) {
                printf("Boton Y presionado\n");
            }else if (GLFW_PRESS == buttons[4]) {
                printf("Boton LB presionado\n");
            }else if (GLFW_PRESS == buttons[5]) {
                printf("Boton RB presionado\n");
            }else if (GLFW_PRESS == buttons[6]) {
                printf("Boton SELECT presionado\n");
            }else if (GLFW_PRESS == buttons[7]) {
                printf("Boton START presionado\n");
            }else if (GLFW_PRESS == buttons[8]) {
                printf("Boton L3 presionado\n");
            }else if (GLFW_PRESS == buttons[9]) {
                printf("Boton R3 presionado\n");
            }else if (GLFW_PRESS == buttons[10]) {
                printf("Boton D-PAD UP presionado\n");
            }else if (GLFW_PRESS == buttons[11]) {
                printf("Boton D-PAD RIGHT presionado\n");
            }else if (GLFW_PRESS == buttons[12]) {
                printf("Boton D-PAD DOWN presionado\n");
            }else if (GLFW_PRESS == buttons[13]) {
                printf("Boton D-PAD LEFT presionado\n");
            }else if (GLFW_RELEASE == buttons[0]) {
                //printf("boton suelto");
            }

            const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
            //std::cout << name << std::endl;
        }




        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        //glfwWaitEvents();
    }

    glfwTerminate();

    return 0;
}