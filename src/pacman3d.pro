TEMPLATE = app
TARGET = pacman3d
QT += opengl

SOURCES = *.cpp
HEADERS = *.hpp

LIBS += -lGLEW

DESTDIR     = ./../
DLLDESTDIR  = ./../
OBJECTS_DIR = ./../tmp
MOC_DIR     = ./../tmp

unix {
  CONFIG += debug
}

macx {
  INCLUDEPATH += /opt/local/include
}

