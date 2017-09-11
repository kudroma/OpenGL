DESTDIR = "D:/programmingStudy/OpenGL"

TARGET = LightCasters

QT += widgets

CONFIG += c++14
CONFIG += console

TEMPLATE = app

INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\include\GLFW
INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\src
INCLUDEPATH += D:\programmingStudy\OpenGL\glad
INCLUDEPATH += C:\OpenGL\glm

LIBS += -LC:\Users\user\Desktop\OpenGL\glfw-3.2.1\build\src -lglfw3
LIBS += -LD:\programmingStudy\OpenGL -lglad
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
    myDemo \
    lightfragmentshader.frag \
    lightvertexshader.vert \
    directional_fragmentshader.frag \
    attenuation_fragmentshader.frag \
    spotlight_fragmentshader.frag \
    smoothspotlight_fragmentshader.frag
