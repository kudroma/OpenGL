#include <iostream>
#include <stdexcept>
#include <QFile>
#include <QDebug>
#include <QUrl>
#include <qmath.h>

#include "including.h"
#include "shaderprogramcompiler.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
bool initializeGLFWandGLAD();

GLFWwindow* window;

int main(int argc, char *argv[])
{
    ShaderProgramCompilerShp program1 = ShaderProgramCompilerShp(new ShaderProgramCompiler());
    program1->setVertexShaderFileName(QUrl("D:/programmingStudy/OpenGL/ShadersTutorial/vertexshader.vert"));
    program1->setFragmentShaderFileName(QUrl("D:/programmingStudy/OpenGL/ShadersTutorial/fragmentshader1.frag"));
    ShaderProgramCompilerShp program2 = ShaderProgramCompilerShp(new ShaderProgramCompiler());
    program2->setVertexShaderFileName(QUrl("D:/programmingStudy/OpenGL/ShadersTutorial/vertexshader.vert"));
    program2->setFragmentShaderFileName(QUrl("D:/programmingStudy/OpenGL/ShadersTutorial/fragmentshader2.frag"));
    ShaderProgramCompilerShp program3 = ShaderProgramCompilerShp(new ShaderProgramCompiler());
    program3->setVertexShaderFileName(QUrl("D:/programmingStudy/OpenGL/ShadersTutorial/vertexshader3.vert"));
    program3->setFragmentShaderFileName(QUrl("D:/programmingStudy/OpenGL/ShadersTutorial/fragmentshader3.frag"));

    if(initializeGLFWandGLAD() &&
            program1->CompileProgram() &&
            program2->CompileProgram() &&
            program3->CompileProgram())
    {
        /// Create VAO1 and VBO1
        ///
        float vertices1[] = {
            -0.3f, -0.0f, 0.0f,
             0.0f,  0.5f, 0.0f,
             0.0f, -0.5f, 0.0f
        };
        unsigned int VAO1, VBO1;
        glGenVertexArrays(1,&VAO1);
        glGenBuffers(1, &VBO1);
        glBindVertexArray(VAO1);

        glBindBuffer(GL_ARRAY_BUFFER,VBO1);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),vertices1,GL_STATIC_DRAW);
        /// set how to interpret vertex data
        ///
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        /// Create VAO2 and VBO2
        ///
        float vertices2[] = {
             0.0f,  0.0f, 0.0f,
             0.3f,  0.5f, 0.0f,
             0.3f, -0.5f, 0.0f
        };
        unsigned int VAO2, VBO2;
        glGenVertexArrays(1,&VAO2);
        glGenBuffers(1, &VBO2);
        glBindVertexArray(VAO2);

        glBindBuffer(GL_ARRAY_BUFFER,VBO2);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2,GL_STATIC_DRAW);
        /// set how to interpret vertex data
        ///
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        /// Create VAO3 and VBO3
        ///
        float vertices3[] = {
            -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        };
        unsigned int VAO3, VBO3;
        glGenVertexArrays(1,&VAO3);
        glGenBuffers(1, &VBO3);
        glBindVertexArray(VAO3);

        glBindBuffer(GL_ARRAY_BUFFER,VBO3);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices3),vertices3,GL_STATIC_DRAW);
        /// set how to interpret vertex data
        ///
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        float timeValue, greenValue;
        int vertexColorLocation;

        unsigned int program1ID, program2ID, program3ID;
        program1ID = program1->programID();
        program2ID = program2->programID();
        program3ID = program3->programID();

        while(!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(185./255.,204./255.,179./255.,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(program1ID);
            glBindVertexArray(VAO1);
            glDrawArrays(GL_TRIANGLES,0,3);

            timeValue = glfwGetTime();
            greenValue = (qSin(timeValue)/2.0f) + 0.5f;
            vertexColorLocation = glGetUniformLocation(program2ID,"ourColor");
            glUseProgram(program2ID);
            glBindVertexArray(VAO2);
            glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
            glDrawArrays(GL_TRIANGLES,0,3);

            glUseProgram(program3ID);
            glBindVertexArray(VAO3);
            glDrawArrays(GL_TRIANGLES,0,3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO3);
        glDeleteBuffers(1, &VBO3);

        glfwTerminate();
    }

    return 0;
}

bool initializeGLFWandGLAD()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
