TARGET = pacman3d

DESTDIR     = ./../
DLLDESTDIR  = ./../
OBJECTS_DIR = ./../tmp
MOC_DIR     = ./../tmp

QT += opengl

CONFIG += debug
CONFIG += warn_on

LIBS += -lGLEW

macx {
  CONFIG -= app_bundle
  INCLUDEPATH += /opt/local/include
}

SOURCES = *.cpp
HEADERS = *.hpp

