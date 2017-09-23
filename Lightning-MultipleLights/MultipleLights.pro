DESTDIR = "D:/programmingStudy/OpenGL"

TARGET = MultipleLights

QT += widgets

CONFIG += c++14
CONFIG += console

TEMPLATE = app

INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\include\GLFW
INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\src
INCLUDEPATH += C:\OpenGL\glad\glad\include
INCLUDEPATH += C:\OpenGL\glm

LIBS += -LC:\Users\user\Desktop\OpenGL\glfw-3.2.1\build\src -lglfw3
LIBS += -LC:\OpenGL\glad\glad -lglad
LIBS += -LC:\Qt\Tools\mingw530_32\i686-w64-mingw32\lib -lopengl32

HEADERS += \
    shaderprogramcompiler.h \
    stb_image.h \
    camera.h

SOURCES += \
    shaderprogramcompiler.cpp \
    main.cpp \
    stb_image.cpp

DISTFILES += \
    vertexshader.vert \
    lightvertexshader.vert \
    fragmentshader.frag \
    lightfragmentshader.frag
