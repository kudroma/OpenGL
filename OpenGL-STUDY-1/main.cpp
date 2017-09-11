#include <iostream>
#include <stdexcept>
#include "glad/glad.h"
#define GLFW_DLL
#include <glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

const char *fragmentShaderSource1 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.2f, 224.0f/255.0f);\n"
    "}\n\0";

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

int main(int argc, char *argv[])
{
    glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
        if (window == NULL){
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
            std::cout << "Failed to initialize OpenGL context" << std::endl;
            return -1;
        }
        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);


        /// create and compile vertex shader
        ///
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
        glCompileShader(vertexShader);
        int success;
        char infolog[512];
        glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader,512,NULL,infolog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
        }
        /// creat and compile fragment shader
        ///
        unsigned int fragmentShader1;
        fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader1,1,&fragmentShaderSource1,NULL);
        glCompileShader(fragmentShader1);
        glGetShaderiv(fragmentShader1,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader1,512,NULL,infolog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
        }

        unsigned int fragmentShader2;
        fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader2,1,&fragmentShaderSource2,NULL);
        glCompileShader(fragmentShader2);
        glGetShaderiv(fragmentShader2,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader2,512,NULL,infolog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
        }

        /// create shader programm
        ///
        unsigned int shaderProgram1;
        shaderProgram1 = glCreateProgram();
        glAttachShader(shaderProgram1,vertexShader);
        glAttachShader(shaderProgram1,fragmentShader1);
        glLinkProgram(shaderProgram1);
        glGetProgramiv(shaderProgram1,GL_LINK_STATUS,&success);
        if(!success){
            glGetProgramInfoLog(shaderProgram1,512,NULL,infolog);
        }
        /// clear shaders because program is ready
        ///
        glDeleteShader(fragmentShader1);

        /// create shader programm
        ///
        unsigned int shaderProgram2;
        shaderProgram2 = glCreateProgram();
        glAttachShader(shaderProgram2,vertexShader);
        glAttachShader(shaderProgram2,fragmentShader2);
        glLinkProgram(shaderProgram2);
        glGetProgramiv(shaderProgram2,GL_LINK_STATUS,&success);
        if(!success){
            glGetProgramInfoLog(shaderProgram2,512,NULL,infolog);
        }
        /// clear shaders because program is ready
        ///
        glDeleteShader(fragmentShader2);
        glDeleteShader(vertexShader);

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

        while(!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(0.7f,0.0f,0.0f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram1);
            glBindVertexArray(VAO1);
            glDrawArrays(GL_TRIANGLES,0,3);

            glUseProgram(shaderProgram2);
            glBindVertexArray(VAO2);
            glDrawArrays(GL_TRIANGLES,0,3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO2);
        glDeleteBuffers(1, &VBO2);

        glfwTerminate();

        return 0;
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
