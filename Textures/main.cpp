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

GLFWwindow* window;
float opacityOfTexture2 = 0.2;

bool initializeGLFWandGLAD();
void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char* argv[])
{
//    QApplication a(argc,argv);
    ShaderProgramCompilerShp program = ShaderProgramCompilerShp(new ShaderProgramCompiler());
    program->setVertexShaderFileName(QUrl("D:/programmingStudy/OpenGL/Textures/vertexshader.vert"));
    program->setFragmentShaderFileName(QUrl("D:/programmingStudy/OpenGL/Textures/fragmentshader.frag"));

    if(initializeGLFWandGLAD() &&
            program->CompileProgram())
    {
        /// create vertices
        ///
        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
        };
        /// create indecies
        ///
        unsigned int indecies[] = {
            0, 1, 3,
            1, 2, 3
        };

        /// create VAO, VBO and EBO
        ///
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indecies),indecies,GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

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
        unsigned char *data = stbi_load("D:/programmingStudy/OpenGL/Textures/container.jpg", &width, &height, &nrChannels, 0);
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
        data = stbi_load("D:/programmingStudy/OpenGL/Textures/face.jpg", &width, &height, &nrChannels, 0);
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

        while(!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(185./255.,204./255.,179./255.,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            glUniform1i(glGetUniformLocation(programID, "texture1"), 0.0); // set it manually
            glUniform1i(glGetUniformLocation(programID, "texture2"), 1.0); // set it manually

            glUseProgram(programID);
            opacityOfSecondTexture = glGetUniformLocation(programID,"opacityOfTexture2");

            glBindVertexArray(VAO);
            glUniform1f(opacityOfSecondTexture,opacityOfTexture2);
            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

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
