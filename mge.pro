TEMPLATE = app

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    glad/src/glad.c \
    src/mge_glbatchbase.cpp \
    src/mge_glshaderprogram.cpp \
    src/mge_gltexture2d.cpp \
    src/mge_glprogrammanager.cpp \
    src/mge_globalfunctions.cpp \
    src/mge_image.cpp \
    src/mge_gluniformmanager.cpp \
    src/mge_glcamera.cpp

HEADERS += \
    src/mge_glbatchbase.h \
    src/mge_glshaderprogram.h \
    src/mge_gltexture2d.h \
    src/mge_glprogrammanager.h \
    src/mge_global.h \
    src/mge_globalfunction.h \
    src/mge_image.h \
    src/mge_gluniformmanager.h \
    src/mge_math.h \
    src/mge_glcamera.h

#for dynamic link sfml library
unix : LIBS += -L$$PWD/sfml/linux64/ -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system
win32 : LIBS += -L$$PWD/sfml/win32/ -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system
INCLUDEPATH += $$PWD/sfml/include
unix : DEPENDPATH += $$PWD/sfml/linux64
win32 : DEPENDPATH += $$PWD/sfml/win32

#glad headers include path
INCLUDEPATH += $$PWD/glad/include
#glm headers include path
INCLUDEPATH += $$PWD/glm

#other dependence
#dl for dynamic resolve function entrance in linux(for glad)
#pthread for std::thread in linux
unix : LIBS += -ldl -lpthread

DISTFILES += \
    README.md \
    issues.md \
    others/test.vert \
    others/test.config \
    others/test.frag \
    others/basic.config \
    others/basic.frag \
    others/basic.vert
