#include <iostream>
#include <glad/glad.h>
#define GLFW_DLL
#include <glfw3.h>
#include "shaderprogramcompiler.h"
#include <QImage>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

GLFWwindow* window;
float opacityOfTexture2 = 0.2;

bool initializeGLFWandGLAD();
void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.1f, 0.1f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightDirection(0.0f, 0.0f, 4.0f);

QString baseDir = "D:/programmingStudy/OpenGL/Lightning-Materials/";

int main(int argc, char* argv[])
{
//    QApplication a(argc,argv);
    ShaderProgramCompilerShp program = ShaderProgramCompilerShp(new ShaderProgramCompiler());
    program->setVertexShaderFileName(QUrl(baseDir + "vertexshader.vert"));
    program->setFragmentShaderFileName(QUrl(baseDir + "fragmentshader.frag"));

    ShaderProgramCompilerShp lightProgram = ShaderProgramCompilerShp(new ShaderProgramCompiler());
    lightProgram->setVertexShaderFileName(QUrl(baseDir + "lightvertexshader.vert"));
    lightProgram->setFragmentShaderFileName(QUrl(baseDir + "lightfragmentshader.frag"));

    if(initializeGLFWandGLAD() &&
            program->CompileProgram() &&
            lightProgram->CompileProgram())
    {
        /// create vertices
        ///
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        /// Create VAO and VBO
        ///
        unsigned int VAO, lightVAO, VBO;
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenVertexArrays(1,&lightVAO);
        glBindVertexArray(lightVAO);
        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        unsigned int programID = program->programID();
        unsigned int lightProgramID = lightProgram->programID();
        unsigned int lightColorLocation = glGetUniformLocation(programID,"lightColor");
        unsigned int modelLocation = glGetUniformLocation(programID,"model");
        unsigned int viewLocation = glGetUniformLocation(programID,"view");
        unsigned int projectionLocation = glGetUniformLocation(programID,"projection");
        unsigned int viewPosLocation  = glGetUniformLocation(programID,"viewPos");
        unsigned int materialAmbientLocation  = glGetUniformLocation(programID,"material.ambient");
        unsigned int materialDiffuseLocation  = glGetUniformLocation(programID,"material.diffuse");
        unsigned int materialSpecularLocation = glGetUniformLocation(programID,"material.specular");
        unsigned int materialShinessLocation  = glGetUniformLocation(programID,"material.shiness");
        unsigned int lightAmbientLocation  = glGetUniformLocation(programID,"light.ambient");
        unsigned int lightDiffuseLocation  = glGetUniformLocation(programID,"light.diffuse");
        unsigned int lightSpecularLocation = glGetUniformLocation(programID,"light.specular");
        unsigned int lightPositionLocation  = glGetUniformLocation(programID,"light.position");

        unsigned int lightModelLocation = glGetUniformLocation(lightProgramID,"model");
        unsigned int lightViewLocation = glGetUniformLocation(lightProgramID,"view");
        unsigned int lightProjectionLocation = glGetUniformLocation(lightProgramID,"projection");

        while(!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            float angle = 0.0001;
            float positionV = 0.2;
            glm::vec3 lightPosAfterRotation = lightDirection;
//            lightPosAfterRotation.x = angle + glm::sin(glfwGetTime()*positionV)*glm::radians(360.0f);
//            lightPosAfterRotation.z = angle + glm::cos(glfwGetTime()*positionV)*glm::radians(360.0f);

            glm::vec3 lightColor;
            float lightV = 0.2;
            lightColor.x = sin(glfwGetTime()*lightV*2.0f);
            lightColor.y = sin(glfwGetTime()*lightV*1.0f);
            lightColor.z = sin(glfwGetTime()*lightV*1.3f);
            glm::vec3 diffuseColor = lightColor*glm::vec3(0.5f);
            glm::vec3 ambientColor = diffuseColor*glm::vec3(0.2f);

            glClearColor(0.25f,0.25f,0.15f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(programID);
            glBindVertexArray(VAO);
            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4();
            glUniform3f(lightColorLocation,lightColor.x,lightColor.y,lightColor.z);
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3f(viewPosLocation, camera.Position.x, camera.Position.y, camera.Position.z);
            glUniform3f(materialAmbientLocation, 1.0f, 0.35f, 0.31f);
            glUniform3f(materialDiffuseLocation, 1.0f, 0.35f, 0.31f);
            glUniform3f(materialSpecularLocation, 0.5f, 0.5f, 0.5f);
            glUniform1f(materialShinessLocation, 32.0f);
            glUniform3f(lightAmbientLocation, ambientColor.x, ambientColor.y, ambientColor.z);
            glUniform3f(lightDiffuseLocation, diffuseColor.x, diffuseColor.y, diffuseColor.z);
            glUniform3f(lightSpecularLocation, 1.0f, 1.0f, 1.0f);
            glUniform3f(lightPositionLocation, lightPosAfterRotation.x, lightPosAfterRotation.y, lightPosAfterRotation.z);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glUseProgram(lightProgramID);
            glBindVertexArray(lightVAO);
            glm::mat4 modelLight = glm::mat4();
            modelLight = glm::translate(modelLight, lightPosAfterRotation);
            modelLight = glm::scale(modelLight, glm::vec3(0.2f));
            glUniformMatrix4fv(lightModelLocation, 1, GL_FALSE, glm::value_ptr(modelLight));
            glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteVertexArrays(1, &lightVAO);
        glDeleteBuffers(1, &VBO);

        glfwTerminate();
    }

    return 0;
}

bool initializeGLFWandGLAD()
{
    if(!glfwInit())
    {
        std::cout << "GLFW wasn't loaded";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE); // I don't know what does this mean
    window = glfwCreateWindow(800,600,"Textures",NULL,NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return false;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    return true;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
