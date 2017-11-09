DESTDIR = "D:/programmingStudy/OpenGL"

TARGET = ModelLoading-Mesh

CONFIG += c++14
CONFIG += console

TEMPLATE = app

INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\include\GLFW
INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\src
INCLUDEPATH += C:\OpenGL\glad\glad\include
INCLUDEPATH += C:\OpenGL\glm
INCLUDEPATH += C:\OpenGL\assimp\assimp-3.3.1\include

LIBS += -LC:\Users\user\Desktop\OpenGL\glfw-3.2.1\build\src -lglfw3
LIBS += -LC:\OpenGL\glad\glad -lglad
LIBS += -LC:\Qt\Tools\mingw530_32\i686-w64-mingw32\lib -lopengl32
LIBS += -LC:\OpenGL\assimp\build\code\Debug -lassimp-vc140-mt

HEADERS += \
    shaderprogramcompiler.h \
    camera.h \
    mesh.h \
    shader.h \
    model.h \
    stb_image.h

SOURCES += \
    shaderprogramcompiler.cpp \
    main.cpp \
    stb_image.cpp

DISTFILES += \
    vertex.vert \
    fragment.frag
