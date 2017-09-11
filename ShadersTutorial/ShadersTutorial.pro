DESTDIR = "D:/programmingStudy/OpenGL"

TARGET = OpenGL-STUDY-1

CONFIG += c++11
CONFIG += console

TEMPLATE = app

INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\include\GLFW
INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\src
INCLUDEPATH += D:\programmingStudy\OpenGL\glad

LIBS += -LC:\Users\user\Desktop\OpenGL\glfw-3.2.1\build\src -lglfw3
LIBS += -LD:\programmingStudy\OpenGL -lglad
LIBS += -LC:\Qt\Tools\mingw530_32\i686-w64-mingw32\lib -lopengl32

SOURCES += \
    main.cpp \
    shaderprogramcompiler.cpp

HEADERS += \
    shaderprogramcompiler.h \
    including.h

DISTFILES += \
    simplevertexshader.vert \
    vertexshader.frag \
    fragmentshader1.frag \
    fragmentshader2.frag \
    vertexshader3.vert \
    fragmentshader3.frag
