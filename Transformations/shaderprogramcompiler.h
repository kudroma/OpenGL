#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QUrl>

class ShaderProgramCompiler
{
public:
    ShaderProgramCompiler();

    QUrl vertexShaderFileName() const;
    void setVertexShaderFileName(const QUrl &vertexShaderFileName);

    QUrl fragmentShaderFileName() const;
    void setFragmentShaderFileName(const QUrl &fragmentShaderFileName);

    bool CompileProgram();

    unsigned int ID() const;

private:
    bool ReadShaderFromFile(QUrl fileName, QByteArray &shaderSource);

private:
    QUrl m_vertexShaderFileName;
    QUrl m_fragmentShaderFileName;
    unsigned int m_ID;
    unsigned int m_vertexShaderID;
    unsigned int m_fragmentShaderID;
    const char* m_fragmentShaderSource;
    const char* m_vertexShaderSource;
};

typedef QSharedPointer<ShaderProgramCompiler> ShaderProgramCompilerShp;
