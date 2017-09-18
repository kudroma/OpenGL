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
glm::vec3 lightDirection(0.0f, 0.0f, -4.0f);

QString baseDir = "D:/programmingStudy/OpenGL/Lightning-MultipleLights/";

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
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        /// Create VAO and VBO
        ///
        unsigned int VAO, lightVAO, VBO;
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        stbi_set_flip_vertically_on_load(true);
        unsigned int texture;
        glGenTextures(1,&texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        int width, height, nrChannels;
        QString path = baseDir + "wood.png";
        unsigned char *data = stbi_load(path.toLatin1().data(),&width,&height,&nrChannels,0);
        if(data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        stbi_set_flip_vertically_on_load(true);
        unsigned int specularTexture;
        glGenTextures(1,&specularTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,specularTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        path = baseDir + "specularMap.png";
        data = stbi_load(path.toLatin1().data(),&width,&height,&nrChannels,0);
        if(data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glGenVertexArrays(1,&lightVAO);
        glBindVertexArray(lightVAO);
        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        /// point lights positions
        ///
        glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        auto programID = program->programID();
        unsigned int modelLocation = glGetUniformLocation(programID,"model");
        unsigned int viewLocation = glGetUniformLocation(programID,"view");
        unsigned int projectionLocation = glGetUniformLocation(programID,"projection");

        unsigned int materialAmbientLocation = glGetUniformLocation(programID,"material.ambient");
        unsigned int materialDiffuseLocation  = glGetUniformLocation(programID,"material.diffuse");
        unsigned int materialSpecularLocation = glGetUniformLocation(programID,"material.specular");
        unsigned int materialShinessLocation  = glGetUniformLocation(programID,"material.shiness");

        unsigned int dirLightAmbientLocation  = glGetUniformLocation(programID,"dirLight.ambient");
        unsigned int dirLightDiffuseLocation  = glGetUniformLocation(programID,"dirLight.diffuse");
        unsigned int dirLightSpecularLocation = glGetUniformLocation(programID,"dirLight.specular");
        unsigned int dirLightDirectionLocation  = glGetUniformLocation(programID,"dirLight.direction");

        auto lightPositionLocation_0 = glGetUniformLocation(programID,"posLight[0].position");
        auto lightConstantLocation_0 = glGetUniformLocation(programID,"posLight[0].constant");
        auto lightLinearLocation_0 = glGetUniformLocation(programID,"posLight[0].linear");
        auto lightQuadraticLocation_0 = glGetUniformLocation(programID,"posLight[0].quadratic");
        auto lightAmbientLocation_0 = glGetUniformLocation(programID,"posLight[0].ambient");
        auto lightDiffuseLocation_0 = glGetUniformLocation(programID,"posLight[0].diffuse");
        auto lightSpecularLocation_0 = glGetUniformLocation(programID,"posLight[0].specular");

        auto lightPositionLocation_1 = glGetUniformLocation(programID,"posLight[1].position");
        auto lightConstantLocation_1 = glGetUniformLocation(programID,"posLight[1].constant");
        auto lightLinearLocation_1 = glGetUniformLocation(programID,"posLight[1].linear");
        auto lightQuadraticLocation_1 = glGetUniformLocation(programID,"posLight[1].quadratic");
        auto lightAmbientLocation_1 = glGetUniformLocation(programID,"posLight[1].ambient");
        auto lightDiffuseLocation_1 = glGetUniformLocation(programID,"posLight[1].diffuse");
        auto lightSpecularLocation_1 = glGetUniformLocation(programID,"posLight[1].specular");

        auto lightPositionLocation_2 = glGetUniformLocation(programID,"posLight[2].position");
        auto lightConstantLocation_2 = glGetUniformLocation(programID,"posLight[2].constant");
        auto lightLinearLocation_2 = glGetUniformLocation(programID,"posLight[2].linear");
        auto lightQuadraticLocation_2 = glGetUniformLocation(programID,"posLight[2].quadratic");
        auto lightAmbientLocation_2 = glGetUniformLocation(programID,"posLight[2].ambient");
        auto lightDiffuseLocation_2 = glGetUniformLocation(programID,"posLight[2].diffuse");
        auto lightSpecularLocation_2 = glGetUniformLocation(programID,"posLight[2].specular");

        auto lightPositionLocation_3 = glGetUniformLocation(programID,"posLight[3].position");
        auto lightConstantLocation_3 = glGetUniformLocation(programID,"posLight[3].constant");
        auto lightLinearLocation_3 = glGetUniformLocation(programID,"posLight[3].linear");
        auto lightQuadraticLocation_3 = glGetUniformLocation(programID,"posLight[3].quadratic");
        auto lightAmbientLocation_3 = glGetUniformLocation(programID,"posLight[3].ambient");
        auto lightDiffuseLocation_3 = glGetUniformLocation(programID,"posLight[3].diffuse");
        auto lightSpecularLocation_3 = glGetUniformLocation(programID,"posLight[3].specular");


        unsigned int lightModelLocation = glGetUniformLocation(lightProgramID,"model");
        unsigned int lightViewLocation = glGetUniformLocation(lightProgramID,"view");
        unsigned int lightProjectionLocation = glGetUniformLocation(lightProgramID,"projection");

        glm::vec3 cubePositions[] = {
          glm::vec3( 0.0f,  0.0f,  0.0f),
          glm::vec3( 2.0f,  5.0f, -15.0f),
          glm::vec3(-1.5f, -2.2f, -2.5f),
          glm::vec3(-3.8f, -2.0f, -12.3f),
          glm::vec3( 2.4f, -0.4f, -3.5f),
          glm::vec3(-1.7f,  3.0f, -7.5f),
          glm::vec3( 1.3f, -2.0f, -2.5f),
          glm::vec3( 1.5f,  2.0f, -2.5f),
          glm::vec3( 1.5f,  0.2f, -1.5f),
          glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        while(!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
            glm::vec3 diffuseColor = lightColor*glm::vec3(0.5f);
            glm::vec3 ambientColor = diffuseColor*glm::vec3(0.2f);

            glClearColor(0.25f,0.25f,0.15f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(programID);
            glBindVertexArray(VAO);
            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

            /// працягнуць тут і перадаць у шэйдэры ўсю неабходную інфармацыю, пасля чаго
            /// праверыць, ці запрацуе праграма

            glUniform3f(viewPosLocation, camera.Position.x, camera.Position.y, camera.Position.z);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,texture);
            glUniform1i(materialDiffuseLocation,0.0f);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,specularTexture);
            glUniform1i(materialSpecularLocation, 1.0);
            glUniform1f(materialShinessLocation, 32.0f);
            glUniform3f(dirLightAmbientLocation, ambientColor.x, ambientColor.y, ambientColor.z);
            glUniform3f(dirLightDiffuseLocation, diffuseColor.x, diffuseColor.y, diffuseColor.z);
            glUniform3f(dirLightSpecularLocation, 1.0f, 1.0f, 1.0f);
            glUniform3f(dirLightDirectionLocation, lightDirection.x, lightDirection.y, lightDirection.z);
//            glUniform3f(lightPositionLocation,lightPosAfterRotation.x,lightPosAfterRotation.y,lightPosAfterRotation.z);
            glUniform3f(lightPositionLocation,lightPosition.x,lightPosition.y,lightPosition.z);
            glUniform1f(lightConstantLocation,1.0f);
            glUniform1f(lightLinearLocation, 0.027f);
            glUniform1f(lightQuadraticLocation, 0.0028f);
            glUniform1f(lightCutOffLocation,glm::cos(glm::radians(12.5f)));
            glUniform1f(lightOuterCutOffLocation,glm::cos(glm::radians(17.5f)));

            for(int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4();
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f*i;
                model = glm::rotate(model,glm::radians(angle),glm::vec3(1.0f,0.3f,0.5f));
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }


//            glUseProgram(lightProgramID);
//            glBindVertexArray(lightVAO);
//            glm::mat4 modelLight = glm::mat4();
//            modelLight = glm::translate(modelLight, lightPosAfterRotation);
//            modelLight = glm::scale(modelLight, glm::vec3(0.2f));
//            glUniformMatrix4fv(lightModelLocation, 1, GL_FALSE, glm::value_ptr(modelLight));
//            glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, glm::value_ptr(view));
//            glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
//            glDrawArrays(GL_TRIANGLES, 0, 36);

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
