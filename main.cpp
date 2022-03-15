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
float camX = 0, camY = 0, camZ = 0, rotX = 0, rotY = 0, rotZ = 0, speed = 0, latspeed = 0;
scene showcase(25,1,0);
int main()
{

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
    Shader renderer("vertex.glsl", "fragment.glsl"); // you can name your shader files however you like

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

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    float * planesarray = new float[showcase.numPlanes*11];

    for(int i = 0;i < showcase.numPlanes; i++)
    {
        planesarray[i] = showcase.planelist[i].PosX;
        planesarray[i+1] = showcase.planelist[i].PosY;
        planesarray[i+2] = showcase.planelist[i].PosZ;

        planesarray[i+3] = showcase.planelist[i].normX;
        planesarray[i+4] = showcase.planelist[i].normY;
        planesarray[i+5] = showcase.planelist[i].normZ;

        planesarray[i+6] = showcase.planelist[i].colourRED;
        planesarray[i+7] = showcase.planelist[i].colourGREEN;
        planesarray[i+8] = showcase.planelist[i].colourBLUE;
        planesarray[i+9] = showcase.planelist[i].transparency;
        planesarray[i+10] = showcase.planelist[i].roughthness;
    }

    float * spheresarray = new float[showcase.numSpheres*9];

    for(int i = 0;i < showcase.numSpheres; i++)
    {
        spheresarray[(9*i)] = showcase.spherelist[i].PosX;
        spheresarray[(9*i)+1] = showcase.spherelist[i].PosY;
        spheresarray[(9*i)+2] = showcase.spherelist[i].PosZ;

        spheresarray[(9*i)+3] = showcase.spherelist[i].Size;

        spheresarray[(9*i)+4] = showcase.spherelist[i].colourRED;
        spheresarray[(9*i)+5] = showcase.spherelist[i].colourGREEN;
        spheresarray[(9*i)+6] = showcase.spherelist[i].colourBLUE;
        spheresarray[(9*i)+7] = showcase.spherelist[i].transparency;
        spheresarray[(9*i)+8] = showcase.spherelist[i].roughthness;
    }

    //this is how i transfer the content of the different object arrays
    //-----------------------------------------------------------------

    //transphering Plane Data:
    //------------------------
    int ParrSize = (4 * showcase.numPlanes*11);
    GLuint PLAssbo;
    glGenBuffers(1, &PLAssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, PLAssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, ParrSize, planesarray, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, PLAssbo);

    //transphering Sphere Data:
    //-------------------------
    int SarrSize = (4 * showcase.numSpheres*9);
    GLuint SPHssbo;
    glGenBuffers(1, &SPHssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SPHssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, SarrSize, spheresarray, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SPHssbo);



//     int CarrSize = (4 * showcase.numPlanes*11);
//     GLuint CUBssbo;
//     glGenBuffers(1, &PLAssbo);
//     glBindBuffer(GL_SHADER_STORAGE_BUFFER, CUBssbo);
//     glBufferData(GL_SHADER_STORAGE_BUFFER, ParrSize, planesarray, GL_DYNAMIC_DRAW);
//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, PLAssbo);
    //------------------------------------------------------------
    glUseProgram(0);
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //emergency stop press EXIT to kill any process
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

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
            renderer.setInt("planesNUM",ParrSize/4);
            renderer.setInt("sphereNUM",SarrSize/4);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // render the triangle using the shader
            renderer.use();
            //we now need to bind the textures
            //--------------------------------
            //the texture containing all the data for the planes

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
    glDeleteBuffers(1, &PLAssbo);
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
