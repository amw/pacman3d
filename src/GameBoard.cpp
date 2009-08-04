#include "GameBoard.hpp"
#include <QFile>


GameBoard::GameBoard( QString name )
  : name( name )
{}

GameBoard::~GameBoard() {
  glDeleteLists( this->wallsList, 1 );
}

bool GameBoard::initialize() {
  QString mapPath( "levels/" );
  mapPath += name;
  mapPath += ".txt";

  QFile map( mapPath );

  if ( ! map.exists() ) {
    return false;
  }

  return true;
}

void GameBoard::initializeGL() {
  this->wallsList = glGenLists( 1 );
  glNewList( this->wallsList, GL_COMPILE );
  glBegin( GL_QUADS );
  {
    glColor3f( 1.0, 0.0, 0.0 );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f,  0.1f);

    glColor3f( 0.0, 1.0, 0.0 );
    glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f, -0.1f);

    glColor3f( 0.0, 0.0, 1.0 );
    glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);

    glColor3f( 1.0, 0.0, 1.0 );
    glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);

    glColor3f( 1.0, 1.0, 1.0 );
    glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);

    glColor3f( 0.0, 1.0, 1.0 );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
  }
  glEnd();
  glEndList();
}

void GameBoard::render() {
  glCallList( this->wallsList );
}

