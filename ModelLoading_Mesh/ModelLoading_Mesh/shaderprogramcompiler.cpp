#include "stdafx.h"

#include "shaderprogramcompiler.h"
#include "glad/glad.h"
#include <iostream>

ShaderProgramCompiler::ShaderProgramCompiler()
{

}

QUrl ShaderProgramCompiler::vertexShaderFileName() const
{
    return m_vertexShaderFileName;
}

void ShaderProgramCompiler::setVertexShaderFileName(const QUrl &vertexShaderFileName)
{
    m_vertexShaderFileName = vertexShaderFileName;
}

QUrl ShaderProgramCompiler::fragmentShaderFileName() const
{
    return m_fragmentShaderFileName;
}

void ShaderProgramCompiler::setFragmentShaderFileName(const QUrl &fragmentShaderFileName)
{
    m_fragmentShaderFileName = fragmentShaderFileName;
}

unsigned int ShaderProgramCompiler::programID() const
{
    return m_programID;
}

bool ShaderProgramCompiler::CompileProgram()
{
    QByteArray vertexByteArray, fragmentByteArray;
    if(ReadShaderFromFile(m_vertexShaderFileName,vertexByteArray)
            && ReadShaderFromFile(m_fragmentShaderFileName,fragmentByteArray))
    {
        /// create and compile vertex shader
        ///
        m_vertexShaderSource = vertexByteArray.constData();
        m_fragmentShaderSource = fragmentByteArray.constData();
        m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vertexShaderID,1,&m_vertexShaderSource,NULL);
        glCompileShader(m_vertexShaderID);
        int success;
        char infolog[512];
        glGetShaderiv(m_vertexShaderID,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(m_vertexShaderID,512,NULL,infolog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
        }

        /// create and compile fragment shader
        ///
        m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragmentShaderID,1,&m_fragmentShaderSource,NULL);
        glCompileShader(m_fragmentShaderID);
        glGetShaderiv(m_fragmentShaderID,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(m_fragmentShaderID,512,NULL,infolog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
        }

        /// create and compile shader programm
        ///
        m_programID = glCreateProgram();
        glAttachShader(m_programID,m_vertexShaderID);
        glAttachShader(m_programID,m_fragmentShaderID);
        glLinkProgram(m_programID);
        glGetProgramiv(m_programID,GL_LINK_STATUS,&success);
        if(!success){
            glGetProgramInfoLog(m_programID,512,NULL,infolog);
        }

        /// clear shaders because program is ready
        ///
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);

        return true;
    }
    else
    {
        return false;
    }
}

bool ShaderProgramCompiler::ReadShaderFromFile(QUrl fileName, QByteArray &shaderSource)
{
    QFile file(fileName.toString());
    if(!file.open(QIODevice::ReadOnly))
        return false;

    while(!file.atEnd())
    {
        shaderSource += file.readLine();
    }

    file.close();

    return true;
}
