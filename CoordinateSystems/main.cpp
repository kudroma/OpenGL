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

GLFWwindow* window;
float opacityOfTexture2 = 0.2;

bool initializeGLFWandGLAD();
void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow *window);
float screenWidth = 800;
float screenHeight = 600;

int main(int argc, char* argv[])
{
//    QApplication a(argc,argv);
    ShaderProgramCompilerShp program = ShaderProgramCompilerShp(new ShaderProgramCompiler());
    program->setVertexShaderFileName(QUrl("D:/programmingStudy/OpenGL/CoordinateSystems/vertexshader.vert"));
    program->setFragmentShaderFileName(QUrl("D:/programmingStudy/OpenGL/CoordinateSystems/fragmentshader.frag"));

    if(initializeGLFWandGLAD() &&
            program->CompileProgram())
    {
        /// create vertices
        ///
        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };

        /// create VAO, VBO and EBO
        ///
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        /// generate textures
        ///
        stbi_set_flip_vertically_on_load(true);
        uint texture1;
        glGenTextures(1,&texture1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char *data = stbi_load("D:/programmingStudy/OpenGL/CoordinateSystems/container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        uint texture2;
        glGenTextures(1,&texture2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        data = stbi_load("D:/programmingStudy/OpenGL/CoordinateSystems/face.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glBindVertexArray(0);

        uint programID = program->programID();
        float opacityOfSecondTexture;

        glEnable(GL_DEPTH_TEST);

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
            processInput(window);

            glClearColor(185./255.,204./255.,179./255.,1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(VAO);
            glUseProgram(programID);

            glm::mat4 view;
            view = glm::translate(view,glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection;
            projection = glm::perspective(glm::radians(45.0f),screenWidth/screenHeight,0.1f, 100.0f);
            int modelLocation = glGetUniformLocation(programID,"model");
            int viewLocation = glGetUniformLocation(programID,"view");
            int projectionLocation = glGetUniformLocation(programID,"projection");

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            glUniform1i(glGetUniformLocation(programID, "texture1"), 0.0); // set it manually
            glUniform1i(glGetUniformLocation(programID, "texture2"), 1.0); // set it manually

            opacityOfSecondTexture = glGetUniformLocation(programID,"opacityOfTexture2");
            glUniform1f(opacityOfSecondTexture,opacityOfTexture2);

            for(unsigned int i = 0; i < 10; i++)
            {
              glm::mat4 model;
              model = glm::translate(model, cubePositions[i]);
              float angle = 20.0f * (i+1);
              model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
              glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
              glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
              glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
              glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        glfwTerminate();
    }

//    /// load images
//    QImage image("D:/programmingStudy/OpenGL/Textures/container.jpg");
//    int width, height, nrChannels;
//    unsigned char *data = image.bits();
//    width = image.width();
//    height = image.height();
//    std::cout << width;

//    if(image.isNull())
//    {
//        std::cout << "Image Viewer","Error Displaying image";
//    }

//    QGraphicsScene scene;
//    QGraphicsView view(&scene);
//    QGraphicsPixmapItem item(QPixmap::fromImage(image));
//    scene.addItem(&item);
//    view.show();
//    return a.exec();
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
    window = glfwCreateWindow(screenWidth,screenHeight,"Textures",NULL,NULL);
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

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    return true;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS){
        opacityOfTexture2 += 0.001f;
        std::cout << "opacity of texture 2: " << opacityOfTexture2 << "\n";
        if(opacityOfTexture2 > 1.0)
            opacityOfTexture2 = 1.0;
    }
    else if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS){
        opacityOfTexture2 -= 0.001f;
        if(opacityOfTexture2 < 0.0)
            opacityOfTexture2 = 0.0;
    }
}
