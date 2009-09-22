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
    glDetachShader( this->id, *i );
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
    qDebug() << "Couldn't compile shader. Shader log:";

    this->printShaderLog( shader);

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
    qDebug() << "Couldn't link program. Log:";
    this->printLog();
    return false;
  }

  if ( ! this->isLinked() ) {
    qDebug() << "Couldn't link program. Log:";
    this->printLog();
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
    glUseProgram( GL_FALSE );
  }

  if ( glGetError() ) {
    return false;
  }

  return true;
}

GLint PipelineProgram::getUniformLocation( const char * name ) {
  return glGetUniformLocation( this->id, name );
}

void PipelineProgram::printShaderLog( GLuint shader ) {
  char msg[ 3000 ];
  int length;
  glGetShaderInfoLog( shader, 2999, & length, msg );
  msg[ length + 1 ] = 0;
  qDebug() << msg;
}

void PipelineProgram::printLog() {
  char msg[ 3000 ];
  int length;
  glGetProgramInfoLog( this->id, 2999, & length, msg );
  msg[ length + 1 ] = 0;
  qDebug() << msg;
}

