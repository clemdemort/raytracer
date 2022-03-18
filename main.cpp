#include "glad/glad.h"
#include "glad/khrplatform.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <memory>
#include "shader.h"
#include "TimeSync.h"
#include "debug.h"
#include "data.h"
#define PI 3.14159

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void ShowFPS(GLFWwindow * window,std::string title, float ElapsedTime,float HOW_MANY_TIMES_A_SECOND);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

TimeSync Vsync; //video sync
TimeSync Titlesync; //speed at which the screen should be refreshed


//initialise the camera position
//------------------------------
float camX = 0, camY = 10, camZ = -20, rotX = 0, rotY = 0, rotZ = 0, speed = 0, latspeed = 0;
scene showcase(50,0);
int main()
{
    printf("test1");
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "loading...", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //VSYNC set to 1 if activated 0 if not 2 if half
    glfwSwapInterval(1);
    // build and compile our shader program
    // ------------------------------------

    Shader renderer("vertex.glsl", "fragment.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions
         3.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  3.0f, 0.0f   // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glBindVertexArray(0);
    float * spheresarray = new float[showcase.numSpheres*9]; //initializing the array to intercept the data -> it must have the right size.
    showcase.ToSSBOData("GET_SPHERE_DATA",spheresarray);
    for(int i = 0; i < showcase.numSpheres*9;i++)
        std::cout<<spheresarray[i]<<std::endl;

    //this is how i transfer the content of the different object arrays
    //-----------------------------------------------------------------

    //transphering Sphere Data:
    //-------------------------
    int SarrSize = (4 * showcase.numSpheres*9);
    GLuint SPHssbo;
    glGenBuffers(1, &SPHssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SPHssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, SarrSize, spheresarray, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SPHssbo);

    //------------------------------------------------------------
    glUseProgram(0); //clearing any program already linked
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //emergency stop press EXIT to kill any process
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        //this will repeat 60 times a second
        if(Vsync.Sync(60)){
            ShowFPS(window,"Raytracer", Vsync.ElapsedTime,4);
            // input
            // -----
            processInput(window);
            glfwGetWindowSize(window,&SCR_WIDTH, &SCR_HEIGHT);
            renderer.setV2Float("iResolution",SCR_WIDTH,SCR_HEIGHT);
            //top down view
            renderer.setV3Float("CameraPos",camX,camY,camZ);// spawn on top
            renderer.setV3Float("CameraRot",rotX,rotY,rotZ); //look down
            renderer.setFloat("Time",glfwGetTime());
            renderer.setInt("sphereNUM",SarrSize/4);
            renderer.setV3Float("planePos",0,0,0);
            renderer.setV3Float("planeNormal",0,1,0);
            renderer.setV3Float("planeColour",0.75,0.75,0.75);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // render the triangle using the shader
            renderer.use();

            //------------------------------------------------------------------------        glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &SPHssbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    rotX = x/SCR_WIDTH;
    rotY = y/SCR_HEIGHT;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        float force = 30;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            force *= force/2.0f;
        speed += force * Vsync.ElapsedTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        float force = 30;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            force *= force/2.0f;
        speed += -force * Vsync.ElapsedTime;
    }

    camX += Vsync.ElapsedTime * speed * sin(rotX) * cos(rotY);
    camY -= Vsync.ElapsedTime * speed * sin(rotY);
    camZ += Vsync.ElapsedTime * speed * cos(rotX) * cos(rotY);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        float force = 30;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            force *= force / 2.0f;
        latspeed -= force * Vsync.ElapsedTime;

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        float force = 30;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            force *= force/2.0f;
        latspeed += force * Vsync.ElapsedTime;
    }
    //if(abs(latspeed) < 10)rotZ = -latspeed / 70;

    camX += Vsync.ElapsedTime * latspeed * sin(rotX + PI / 2);
    camZ += Vsync.ElapsedTime * latspeed * cos(rotX + PI / 2);


    speed -= Vsync.ElapsedTime * 2.0 * speed;
    latspeed -= Vsync.ElapsedTime * 2.0 * latspeed;


    if (glfwGetKey(window, GLFW_KEY_TAB) != GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
void ShowFPS(GLFWwindow * window,std::string title, float ElapsedTime,float HOW_MANY_TIMES_A_SECOND)
{
    if (Titlesync.Sync(HOW_MANY_TIMES_A_SECOND)) {
                //below you can set the window title
                std::stringstream ss;
                ss << int(1.0 / ElapsedTime);
                std::string temp = ss.str();
                std::string temp2 = title + " -FPS:" + temp;
                char* FPS = (char*)temp2.c_str();
                glfwSetWindowTitle(window, FPS);
            }
}
