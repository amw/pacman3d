TARGET = pacman3d

DESTDIR     = ./../
DLLDESTDIR  = ./../
OBJECTS_DIR = ./../tmp
MOC_DIR     = ./../tmp

QT += opengl

CONFIG += debug
CONFIG += warn_on

LIBS += -lGLEW

QMAKE_CXXFLAGS_RELEASE += -Wall -g0 -O3
QMAKE_CXXFLAGS_DEBUG   += -Wall -ggdb -O0

macx {
  CONFIG -= app_bundle
  INCLUDEPATH += /opt/local/include
}

SOURCES = *.cpp
HEADERS = *.hpp

