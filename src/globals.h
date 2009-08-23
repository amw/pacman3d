#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <QDebug>

#define CHECK_GL_STATUS( function ) \
  { \
    GLenum errorCode = glGetError(); \
    while ( GL_NO_ERROR != errorCode ) { \
      const GLubyte * errorString = gluErrorString( errorCode ); \
      QString error( "Got OpenGL error when calling %1.\nError: %2." ); \
      qDebug() << error \
        .arg( #function ) \
        .arg( ( char * )errorString  ); \
      errorCode = glGetError(); \
    } \
  }

#define CALL_GL( function ) \
  function; \
  CHECK_GL_STATUS( function )

#endif

