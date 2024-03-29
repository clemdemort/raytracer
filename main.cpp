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
#include "objects.h"
#define PI 3.14159

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void ShowFPS(GLFWwindow *window, std::string title, float ElapsedTime, float HOW_MANY_TIMES_A_SECOND);

// Debugging
int getNormals = 0;

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

TimeSync Vsync;     // video sync
TimeSync Psync;     //"physics" sync
TimeSync Titlesync; // speed at which the title should be refreshed

// initialise the camera position
//------------------------------
float camX = 0, camY = 10, camZ = -20, rotX = 0, rotY = 0, rotZ = 0, speed = 0, latspeed = 0;
scene showcase; // initializing the scene
int main()
{
    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 202002L) std::cout << "C++20\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";	
    else std::cout << "pre-standard C++\n";
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Raytracer", NULL, NULL);
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
    // VSYNC set to 1 if activated 0 if not 2 if half
    glfwSwapInterval(1);
    // build and compile our shader program
    // ------------------------------------

    Shader renderer("vertex.glsl", "fragment.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // the triangle is going to act as a canvas for everything we render un=sing the fragment shader
    float vertices[] = {
        // positions
        3.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f, 3.0f, 0.0f   // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer, and then configure vertex attributes.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // glBindVertexArray(0);

    int width = 64, height = 64, depth = 64;

    // passing the voxelspace in a texture3D
    GLuint voxATLAS = 0;           // declaring my texture3D
    glDeleteBuffers(1, &voxATLAS); // in case it hadn't properly been done before
    glGenTextures(1, &voxATLAS);
    glBindTexture(GL_TEXTURE_3D, voxATLAS);
    glTexStorage3D(GL_TEXTURE_3D,
                   1,       // No mipmaps
                   GL_R8UI, // Internal format
                   1024, 1024, 1024);
    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
    // creating a 3D texture to send it to the GPU
    
    //populating the scene with various objects	
    populatescene(showcase,voxATLAS);
    // this is how i transfer the content of the different object arrays
    //-----------------------------------------------------------------
    float *spheresarray;
    showcase.getspheredata(spheresarray);

    float *cubesarray;
    showcase.getcubedata(cubesarray);

    float *voxelsarray;
    showcase.getvoxeldata(voxelsarray);

    uint32_t *palettesarray;
    showcase.getpalettedata(palettesarray);	
    // transphering Sphere Data:
    //-------------------------
    int SarrSize = (4 * showcase.numSpheres * 9);
    GLuint SPHssbo;
    glGenBuffers(1, &SPHssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SPHssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, SarrSize, spheresarray, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SPHssbo);

    // transphering Cube Data:
    //-------------------------
    int CarrSize = (4 * showcase.numCubes * 14);
    GLuint CUBssbo;
    glGenBuffers(1, &CUBssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, CUBssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, CarrSize, cubesarray, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, CUBssbo);
    //-------------------------

    // transphering Voxel Data:
    //-------------------------
    int VarrSize = (4 * showcase.numVoxels * 16);
    GLuint VOXssbo;
    glGenBuffers(1, &VOXssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VOXssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, VarrSize, voxelsarray, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, VOXssbo);
    //-------------------------

    // transphering Palette Data:
    //-------------------------
    int ParrSize = (4 * showcase.numPalettes * 256);
    GLuint PALssbo;
    glGenBuffers(1, &PALssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, PALssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, ParrSize, palettesarray, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, PALssbo);
    //-------------------------


    glUseProgram(0); // clearing any program already linked
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // emergency stop press EXIT to kill any process
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        // this will repeat 60 times a second
        if (Psync.Sync(60))
        { // we can update it even slower to leave more performance for rendering.
            for (int i = 0; i < showcase.numSpheres; i++)
            {
                spheresarray[1 + (i * 9)] += 0.2 * (sin(spheresarray[0 + (i * 9)] + spheresarray[2 + (i * 9)] + glfwGetTime())); // this modifies the Y pos
            }
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, SPHssbo);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, SarrSize, spheresarray);

            for (int i = 0; i < showcase.numCubes; i++)
            {
                cubesarray[6 + (i * 14)] = 0.3 * (cubesarray[0 + (i * 14)] + cubesarray[2 + (i * 14)] + glfwGetTime()); // these are the value for rotation X
                cubesarray[7 + (i * 14)] = 0.3 * (cubesarray[0 + (i * 14)] + cubesarray[2 + (i * 14)] + glfwGetTime()); // Y
                cubesarray[8 + (i * 14)] = 0.3 * (cubesarray[0 + (i * 14)] + cubesarray[2 + (i * 14)] + glfwGetTime()); // and Z
            }
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, CUBssbo);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, CarrSize, cubesarray);

            // make the voxel objects spin around!
            /*for (int i = 0; i < showcase.numVoxels; i++)
            {
                voxelsarray[6 + (i * 16)] = 0.3 * (voxelsarray[0 + (i * 16)] + voxelsarray[2 + (i * 16)] + glfwGetTime()); // these are the value for rotation X
                voxelsarray[7 + (i * 16)] = 0.3 * (voxelsarray[0 + (i * 16)] + voxelsarray[2 + (i * 16)] + glfwGetTime()); // Y
                voxelsarray[8 + (i * 16)] = 0.3 * (voxelsarray[0 + (i * 16)] + voxelsarray[2 + (i * 16)] + glfwGetTime()); // and Z
            }

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, VOXssbo);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, VarrSize, voxelsarray);*/
        }
        if (Vsync.Sync(100))
        { // found out that this makes the framerate a bit more stable in some cases
            ShowFPS(window, "Raytracer", Vsync.ElapsedTime, 4);
            // input
            // -----
            processInput(window);
            glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
            renderer.setV2Float("iResolution", SCR_WIDTH, SCR_HEIGHT);
            renderer.setV3Float("CameraPos", camX, camY, camZ);
            renderer.setV3Float("CameraRot", rotX, rotY, rotZ);
            renderer.setFloat("Time", glfwGetTime());
            renderer.setFloat("Rand", getrand(-1000, 1000) / 1.0);
            renderer.setInt("sphereNUM", showcase.numSpheres);
            renderer.setInt("cubeNUM", showcase.numCubes);
            renderer.setInt("voxelNUM", showcase.numVoxels);
            renderer.setInt("getNormals", getNormals); // debug
            renderer.setV3Float("sunDir", cos(glfwGetTime() / 20.0), 0.75 * (1 + sin(glfwGetTime() / 20.0)), sin(-glfwGetTime() / 20.0));
            renderer.setV3Float("planePos", 0, 0, 0);
            renderer.setV3Float("planeNormal", 0, 1, 0);
            renderer.setV3Float("planeColour", 0.75, 0.75, 0.75);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // render the triangle using the shader
            renderer.use();
            // binding the texture before swapping the buffers
            glBindImageTexture(4,
                               voxATLAS,
                               0,
                               true,
                               0,
                               GL_READ_ONLY,
                               GL_R8UI);

            //------------------------------------------------------------------------
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    printf("deleting buffers...\n");
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &SPHssbo);
    glDeleteBuffers(1, &CUBssbo);
    glDeleteBuffers(1, &VOXssbo);
    glDeleteBuffers(1, &voxATLAS);

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
    rotX = x / SCR_WIDTH;
    rotY = y / SCR_HEIGHT;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        float force = 30;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            force *= force / 2.0f;
        speed += force * Vsync.ElapsedTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        float force = 30;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            force *= force / 2.0f;
        speed += -force * Vsync.ElapsedTime;
    }

    camX += Vsync.ElapsedTime * speed * sin(rotX) * cos(rotY);
    camY -= Vsync.ElapsedTime * speed * sin(rotY);
    camZ += Vsync.ElapsedTime * speed * cos(rotX) * cos(rotY);

    if (camY <= 0.1)
    {
        camY = 0.1;
    }
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
            force *= force / 2.0f;
        latspeed += force * Vsync.ElapsedTime;
    }

    camX += Vsync.ElapsedTime * latspeed * sin(rotX + PI / 2);
    camZ += Vsync.ElapsedTime * latspeed * cos(rotX + PI / 2);

    speed -= Vsync.ElapsedTime * 2.0 * speed;
    latspeed -= Vsync.ElapsedTime * 2.0 * latspeed;

    if (glfwGetKey(window, GLFW_KEY_TAB) != GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        getNormals = 1;
    }
    else
    {
        getNormals = 0;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void ShowFPS(GLFWwindow *window, std::string title, float ElapsedTime, float HOW_MANY_TIMES_A_SECOND)
{
    if (Titlesync.Sync(HOW_MANY_TIMES_A_SECOND))
    {
        std::stringstream ss;
        ss << int(1.0 / ElapsedTime);
        std::string temp = ss.str();
        std::string temp2 = title + " -FPS:" + temp;
        char *FPS = (char *)temp2.c_str();
        glfwSetWindowTitle(window, FPS);
    }
}
