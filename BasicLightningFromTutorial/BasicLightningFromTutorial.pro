DESTDIR = "D:/programmingStudy/OpenGL"

TARGET = BasicLightningFromTutorial

QT += widgets

CONFIG += c++11
CONFIG += console

TEMPLATE = app

INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\include\GLFW
INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\src
INCLUDEPATH += D:\programmingStudy\OpenGL\glad
INCLUDEPATH += C:\OpenGL\glm

LIBS += -LC:\Users\user\Desktop\OpenGL\glfw-3.2.1\build\src -lglfw3
LIBS += -LD:\programmingStudy\OpenGL -lglad
LIBS += -LC:\Qt\Tools\mingw530_32\i686-w64-mingw32\lib -lopengl32

SOURCES += \
    main.cpp

HEADERS += \
    shader_m.h \
    camera.h

DISTFILES += \
    vertexshader.vert \
    fragmentshader.frag \
    lightvertexshader.vert \
    lightfragmentshader.frag
