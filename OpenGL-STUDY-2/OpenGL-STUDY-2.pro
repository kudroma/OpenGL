CONFIG += c++11

TARGET = OpenGL-STUDY-2

DESTDIR = $PWD/..

TEMPLATE = app

#CONFIG += console

INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\include\GLFW
INCLUDEPATH += C:\Users\user\Desktop\OpenGL\glfw-3.2.1\src
INCLUDEPATH += D:\programmingStudy\OpenGL\glad

LIBS += -LC:\Users\user\Desktop\OpenGL\glfw-3.2.1\build\src -lglfw3
LIBS += -LD:\programmingStudy\OpenGL -lglad
LIBS += -LC:\Qt\Tools\mingw530_32\i686-w64-mingw32\lib -lopengl32

TEMPLATE = app

SOURCES += main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
