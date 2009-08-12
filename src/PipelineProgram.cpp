#include "PipelineProgram.hpp"
#include <QFile>
#include <QByteArray>
#include <QDebug>


PipelineProgram::PipelineProgram() {
  this->id = glCreateProgram();
}

PipelineProgram::~PipelineProgram() {
  QVector< GLuint >::iterator i;

  for ( i = this->shaders.begin(); i != this->shaders.end(); ++i ) {
    glDeleteShader( *i );
  }

  glDeleteProgram( this->id );
}

bool PipelineProgram::attachShader( GLenum type, QString filename ) {
  QFile sourceFile( filename );

  if ( ! sourceFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return false;
  }

  QByteArray source = sourceFile.readAll();
  const GLchar* sourceArray = source.constData();

  GLuint shader = glCreateShader( type );

  glShaderSource( shader, 1, & sourceArray, NULL );

  if ( glGetError() ) {
    qDebug() << "Couldn't load shader source.";
    glDeleteShader( shader );
    return false;
  }

  glCompileShader( shader );

  if ( glGetError() ) {
    qDebug() << "Bad compile request.";
    glDeleteShader( shader );
    return false;
  }

  int compileStatus;
  glGetShaderiv( shader, GL_COMPILE_STATUS, & compileStatus );

  if ( GL_FALSE == compileStatus ) {
    qDebug() << "Couldn't compile shader.";
    glDeleteShader( shader );
    return false;
  }

  glAttachShader( this->id, shader );

  if ( glGetError() ) {
    qDebug() << "Couldn't attach shader.";
    glDeleteShader( shader );
    return false;
  }

  this->shaders.append( shader );

  return true;
}

bool PipelineProgram::link() {
  if ( ! this->shaders.size() ) {
    qDebug() << "No shaders attached.";
    return false;
  }

  glLinkProgram( this->id );

  if ( glGetError() ) {
    qDebug() << "Couldn't link program.";
    return false;
  }

  if ( ! this->isLinked() ) {
    qDebug() << "Couldn't link program.";
    return false;
  }

  return true;
}

bool PipelineProgram::isLinked() {
  int linkStatus;
  glGetProgramiv( this->id, GL_LINK_STATUS, & linkStatus );
  return GL_TRUE == linkStatus;
}

bool PipelineProgram::enable( bool enable ) {
  if ( enable ) {
    if ( ! this->isLinked() ) {
      return false;
    }
    glUseProgram( this->id );
  }
  else {
    glUseProgram( 0 );
  }

  if ( glGetError() ) {
    return false;
  }

  return true;
}

