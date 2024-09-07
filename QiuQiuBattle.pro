#-------------------------------------------------
#
# Project created by QtCreator 2024-07-07T19:18:44
#
#-------------------------------------------------
CONFIG += c++14
QT       += core gui
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QiuQiuBattle
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Packaged/include.cpp \
    Packaged/texture2d.cpp \
    canvas.cpp \
    Game/game.cpp \
    Game/globalsource.cpp \
    Game/map.cpp \
    Game/food.cpp \
    Packaged/vec.cpp \
    Packaged/other.cpp \
    Game/ball.cpp \
    Game/spore.cpp \
    Game/role.cpp \
    Game/controller.cpp \
    Game/actionbar.cpp \
    Packaged/timer.cpp \
    Game/ai.cpp \
    Game/boom.cpp \
    Game/effect.cpp \
    Game/particleeffect.cpp \
    Packaged/particlesystem.cpp \
    Game/meteorshowereffect.cpp \
    Game/vortexeffect.cpp \
    Packaged/ElementBufferObj.cpp \
    Packaged/Shaderprogram.cpp \
    Packaged/ShaderStorageBufferObj.cpp \
    Packaged/VertexArrayObj.cpp \
    Packaged/VertexBufferObj.cpp

HEADERS += \
        mainwindow.h \
    Packaged/include.h \
    Packaged/texture2d.h \
    canvas.h \
    Game/game.h \
    Game/globalsource.h \
    Game/map.h \
    Game/food.h \
    Packaged/vec.h \
    Packaged/other.h \
    Game/ball.h \
    Packaged/tree.hpp \
    Game/spore.h \
    Game/role.h \
    Game/controller.h \
    Game/actionbar.h \
    Packaged/timer.h \
    Game/ai.h \
    Game/boom.h \
    Game/effect.h \
    Game/particleeffect.h \
    Packaged/particlesystem.h \
    Game/meteorshowereffect.h \
    Game/vortexeffect.h \
    Packaged/ElementBufferObj.h \
    Packaged/Shaderprogram.h \
    Packaged/ShaderStorageBufferObj.h \
    Packaged/VertexArrayObj.h \
    Packaged/VertexBufferObj.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    src/Shader/FoodRender.frag \
    src/Shader/FoodRender.vert \
    src/Shader/Test.vert \
    src/Shader/BallSurfaceRender.frag \
    src/Shader/BallSurfaceRender.vert \
    src/Shader/ActionBar.frag \
    src/Shader/BallSurfaceRender.frag \
    src/Shader/FoodRender.frag \
    src/Shader/ActionBar.vert \
    src/Shader/BallSurfaceRender.vert \
    src/Shader/FoodRender.vert \
    src/Shader/ActionBar.frag \
    src/Shader/BallSurfaceRender.frag \
    src/Shader/CrossHair.frag \
    src/Shader/FoodRender.frag \
    src/Shader/ActionBar.vert \
    src/Shader/BallSurfaceRender.vert \
    src/Shader/CrossHair.vert \
    src/Shader/FoodRender.vert \
    src/Shader/ActionBar.frag \
    src/Shader/BallSurfaceRender.frag \
    src/Shader/CrossHair.frag \
    src/Shader/FoodRender.frag \
    src/Shader/SporeRender.frag \
    src/Shader/ActionBar.vert \
    src/Shader/BallSurfaceRender.vert \
    src/Shader/CrossHair.vert \
    src/Shader/FoodRender.vert \
    src/Shader/SporeRender.vert \
    src/Shader/ActionBar.frag \
    src/Shader/BallSurfaceRender.frag \
    src/Shader/CrossHair.frag \
    src/Shader/FoodRender.frag \
    src/Shader/SporeRender.frag \
    src/Shader/TextureDraw.frag \
    src/Shader/ActionBar.vert \
    src/Shader/BallSurfaceRender.vert \
    src/Shader/CrossHair.vert \
    src/Shader/FoodRender.vert \
    src/Shader/SporeRender.vert \
    src/Shader/TextureDraw.vert \
    src/Shader/MeteorShowerTransform.geom \
    src/Shader/ParticleTransform.geom \
    src/Shader/VortexTransform.geom \
    src/Shader/ActionBar.frag \
    src/Shader/BallSurfaceRender.frag \
    src/Shader/BoomRender.frag \
    src/Shader/CrossHair.frag \
    src/Shader/FoodRender.frag \
    src/Shader/MeteorShowerRender.frag \
    src/Shader/ParticleRender.frag \
    src/Shader/SporeRender.frag \
    src/Shader/TextureDraw.frag \
    src/Shader/VortexRender.frag \
    src/Shader/WinTipRender.frag \
    src/Shader/ActionBar.vert \
    src/Shader/BallSurfaceRender.vert \
    src/Shader/BoomRender.vert \
    src/Shader/CrossHair.vert \
    src/Shader/FoodRender.vert \
    src/Shader/MeteorShowerRender.vert \
    src/Shader/MeteorShowerTransform.vert \
    src/Shader/Particle.vert \
    src/Shader/ParticleRender.vert \
    src/Shader/ParticleTransform.vert \
    src/Shader/SporeRender.vert \
    src/Shader/TextureDraw.vert \
    src/Shader/VortexRender.vert \
    src/Shader/VortexTransform.vert \
    src/Shader/WinTipRender.vert
