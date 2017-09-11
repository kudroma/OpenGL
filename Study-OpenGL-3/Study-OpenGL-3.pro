DESTDIR = "D:/programmingStudy/OpenGL"

TARGET = OpenGL-STUDY-3

CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\include\GLFW
INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\src
INCLUDEPATH += C:\OpenGL\gl3w-master\gl3w-master\build\include
INCLUDEPATH += D:\programmingStudy\OpenGL\gl3w

LIBS += -LC:\Users\user\Desktop\OpenGL\glfw-3.2.1\build\src -lglfw3
LIBS += -LD:\programmingStudy\OpenGL -lgl3w
LIBS += -LC:\Qt\Tools\mingw530_32\i686-w64-mingw32\lib -lopengl32

SOURCES += \
    main.cpp

HEADERS +=
