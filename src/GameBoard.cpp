#include "GameBoard.hpp"
#include <QTextStream>
#include <QImage>
#include <QDebug>
#include <string.h>


GameBoard::GameBoard( QString name )
  : name( name ),
    allocatedBlocks( false )
{}

GameBoard::~GameBoard() {
  if ( this->allocatedBlocks ) {
    for ( int y = 0; y < this->height; ++y ) {
      delete[] this->blocks[ y ];
    }
    delete[] this->blocks;
  }

  glDeleteLists( this->wallsList, 1 );
}

bool GameBoard::initialize() {
  if ( this->allocatedBlocks ) {
    return false;
  }

  QString mapPath( "levels/" );
  mapPath += name;
  mapPath += ".txt";

  QFile map( mapPath );

  if ( ! map.exists() ) {
    return false;
  }

  if ( ! map.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return false;
  }

  if ( ! this->readBoardSize( map ) ) {
    return false;
  }

  if ( ! this->readBoardBlocks( map ) ) {
    return false;
  }

  return true;
}

bool GameBoard::readBoardSize( QFile & map ) {
  if ( ! map.seek( 0 ) ) {
    return false;
  }

  this->width = 0;
  this->height = 0;

  QTextStream in( &map );

  QString line = in.readLine();
  while ( ! line.isNull() ) {
    int length = line.length();

    if ( length > this->width ) {
      this->width = length;
    }

    line = in.readLine();
    ++this->height;
  }

  if ( 5 > this->height || 5 > this->width ) {
    return false;
  }

  this->width += 2;
  this->height += 2;

  return true;
}

void GameBoard::initBlocksToWalls() {
  int* rowOfWalls = new int[ this->width ];
  int rowBytes = this->width * sizeof( int );

  for ( int x = 0; x < this->width; ++x ) {
    rowOfWalls[ x ] = GameBoard::Wall;
  }

  this->blocks = new int*[ this->height ];

  for ( int y = 0; y < this->height; ++y ) {
    this->blocks[ y ] = new int[ this->width ];
    memcpy( this->blocks[ y ], rowOfWalls, rowBytes );
  }

  this->allocatedBlocks = true;

  delete[] rowOfWalls;
}

bool GameBoard::readBoardBlocks( QFile & map ) {
  if ( ! map.seek( 0 ) ) {
    return false;
  }

  this->initBlocksToWalls();

  bool foundPlayerStart = false;

  QTextStream in( &map );
  QString line = in.readLine();

  int y = this->height - 2;
  while ( ! line.isNull() ) {
    for ( int x = 1; x < this->width - 1; ++x ) {
      int element;

      switch ( line.at( x - 1 ).toAscii() ) {
        case BLOCK_DOT:
          element = GameBoard::Dot;
          break;
        case BLOCK_WALL:
          element = GameBoard::Wall;
          break;
        case BLOCK_PATH:
          element = GameBoard::Path;
          break;
        case BLOCK_POWERUP:
          element = GameBoard::Powerup;
          break;
        case BLOCK_TELEPORT_1:
          element = GameBoard::Teleport1;
          break;
        case BLOCK_PLAYER_START:
          if ( foundPlayerStart ) {
            qDebug() << "Only one player start position allowed.";
            return false;
          }
          foundPlayerStart = true;
          this->player1Start = QPoint( x, y );
          element = GameBoard::PlayerStart;
          break;
        case BLOCK_GHOSTS_START:
          element = GameBoard::GhostsStart;
          this->ghostStarts.append( QPoint( x, y ) );
          break;
        case BLOCK_PLAYER_WALL:
          element = GameBoard::PlayerWall;
          break;
        default:
          qDebug() << "Unknown board block.";
          return false;
      }

      this->blocks[ y ][ x ] = element;
    }

    line = in.readLine();
    --y;
  }

  if ( ! foundPlayerStart || ! this->ghostStarts.size() ) {
    qDebug() << "Missing player's or ghosts' start positions.";
    return false;
  }

  return true;
}

void GameBoard::initializeGL( QGLWidget & target ) {
  QImage wall( "textures/wall.jpg" );
  this->wallTexture = target.bindTexture( wall );
  QImage grass( "textures/grass.jpg" );
  this->grassTexture = target.bindTexture( grass );
  QImage roof( "textures/roof.jpg" );
  this->roofTexture = target.bindTexture( roof );
  QImage tiles( "textures/tiles.jpg" );
  this->ghostsStartTexture = target.bindTexture( tiles );

  this->wallsList = glGenLists( 1 );
  glNewList( this->wallsList, GL_COMPILE );
  {
    for ( int y = 0; y < this->height; ++y ) {
      for ( int x = 0; x < this->width; ++x ) {
        if ( GameBoard::Wall == this->blocks[ y ][ x ] ) {
          this->addWallBlock( x, y );
        }
        else if ( GameBoard::Path == this->blocks[ y ][ x ] ) {
          this->addFloorBlock( x, y, this->grassTexture );
        }
        else if ( GameBoard::Dot == this->blocks[ y ][ x ] ) {
          this->addFloorBlock( x, y, this->grassTexture );
        }
        else if ( GameBoard::Powerup == this->blocks[ y ][ x ] ) {
          this->addFloorBlock( x, y, this->grassTexture );
        }
        else if ( GameBoard::Teleport1 == this->blocks[ y ][ x ] ) {
          this->addFloorBlock( x, y, this->grassTexture );
        }
        else if ( GameBoard::PlayerWall == this->blocks[ y ][ x ] ) {
          this->addFloorBlock( x, y, this->roofTexture );
        }
        else if ( GameBoard::PlayerStart == this->blocks[ y ][ x ] ) {
          this->addFloorBlock( x, y, this->grassTexture );
        }
        else if ( GameBoard::GhostsStart == this->blocks[ y ][ x ] ) {
          this->addFloorBlock(
            x, y, this->ghostsStartTexture
          );
        }
      }
    }
  }
  glEndList();
}

void GameBoard::addWallBlock( int x, int y ) {
  double x1 = BLOCK_WIDTH * x;
  double x2 = x1 + BLOCK_WIDTH;

  double y1 = BLOCK_WIDTH * y;
  double y2 = y1 + BLOCK_WIDTH;

  double z1 = - BLOCK_WIDTH;
  double z2 = + BLOCK_WIDTH;

  double s1 = (double) x         / (double) this->width  * 4.0f;
  double s2 = (double) ( x + 1 ) / (double) this->width  * 4.0f;
  double t1 = (double) y         / (double) this->height * 4.0f;
  double t2 = (double) ( y + 1 ) / (double) this->height * 4.0f;

  glBindTexture( GL_TEXTURE_2D, this->roofTexture );
  glBegin( GL_QUADS );
  {
    glTexCoord2f( s1, t1 ); glVertex3d( x1, y1, z2 );
    glTexCoord2f( s2, t1 ); glVertex3d( x2, y1, z2 );
    glTexCoord2f( s2, t2 ); glVertex3d( x2, y2, z2 );
    glTexCoord2f( s1, t2 ); glVertex3d( x1, y2, z2 );
  }
  glEnd();

  glBindTexture( GL_TEXTURE_2D, this->wallTexture );
  glBegin( GL_QUADS );
  {
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x1, y1, z1 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x1, y2, z1 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x2, y2, z1 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x2, y1, z1 );

    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x1, y2, z1 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x1, y2, z2 );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x2, y2, z2 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x2, y2, z1 );

    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x1, y1, z1 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x2, y1, z1 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x2, y1, z2 );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x1, y1, z2 );

    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x2, y1, z1 );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x2, y2, z1 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x2, y2, z2 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x2, y1, z2 );

    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x1, y1, z1 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x1, y1, z2 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x1, y2, z2 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x1, y2, z1 );
  }
  glEnd();
}

void GameBoard::addFloorBlock( int x, int y, GLuint texture ) {
  double x1 = BLOCK_WIDTH * x;
  double x2 = x1 + BLOCK_WIDTH;

  double y1 = BLOCK_WIDTH * y;
  double y2 = y1 + BLOCK_WIDTH;

  double z1 = - BLOCK_WIDTH;
  double z2 = 0.0f;

  glBindTexture( GL_TEXTURE_2D, texture );
  glBegin( GL_QUADS );
  {
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x1, y1, z2 );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x2, y1, z2 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x2, y2, z2 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x1, y2, z2 );

    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x1, y1, z1 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x1, y2, z1 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x2, y2, z1 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x2, y1, z1 );

    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x1, y2, z1 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x1, y2, z2 );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x2, y2, z2 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x2, y2, z1 );

    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x1, y1, z1 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x2, y1, z1 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x2, y1, z2 );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x1, y1, z2 );

    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x2, y1, z1 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x2, y2, z1 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x2, y2, z2 );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x2, y1, z2 );

    glTexCoord2f( 0.0f, 0.0f ); glVertex3d( x1, y1, z1 );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3d( x1, y1, z2 );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3d( x1, y2, z2 );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3d( x1, y2, z1 );
  }
  glEnd();
}


void GameBoard::render( QGLWidget & ) {
  glCallList( this->wallsList );
}

QPoint GameBoard::getSize() {
  return QPoint( this->width, this->height );
}

QPointF GameBoard::getRealSize() {
  return QPointF( this->width * BLOCK_WIDTH, this->height * BLOCK_WIDTH );
}

QPointF GameBoard::getPlayer1Start() {
  return QPointF(
    this->player1Start.x() * BLOCK_WIDTH + 0.5f,
    this->player1Start.y() * BLOCK_WIDTH + 0.5f
  );
}

bool GameBoard::isAccessibleByGhost( int x, int y ) {
  if (
    this->blocks[ y ][ x ] != GameBoard::Wall
  ) {
    return true;
  }
  else {
    return false;
  }
}

bool GameBoard::isAccessibleByPlayer( int x, int y ) {
  if (
    this->blocks[ y ][ x ] != GameBoard::Wall &&
    this->blocks[ y ][ x ] != GameBoard::PlayerWall
  ) {
    return true;
  }
  else {
    return false;
  }
}

