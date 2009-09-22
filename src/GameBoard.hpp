#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include "Material.hpp"
#include "gl.h"
#include <QGLWidget>
#include <QString>
#include <QFile>
#include <QPoint>
#include <QPointF>

#define BLOCK_WALL 'X'
#define BLOCK_DOT '.'
#define BLOCK_PATH ' '
#define BLOCK_POWERUP 'S'
#define BLOCK_TELEPORT_1 '1'
#define BLOCK_PLAYER_START '_'
#define BLOCK_GHOSTS_START 'G'
#define BLOCK_PLAYER_WALL '='

#define BLOCK_WIDTH 1.0f

class GameBoard : public QObject {
  Q_OBJECT

  public:
    GameBoard( QString name );
    ~GameBoard();
    bool initialize();
    void initializeGL( QGLWidget & target );
    void render( QGLWidget & target );
    QPoint getSize() const;
    QPointF getRealSize() const;
    QPointF getPlayer1Start() const;
    QVector< QPointF > getGhostStarts() const;
    bool isAccessibleByPlayer( int x, int y ) const;
    bool isAccessibleByGhost( int x, int y ) const;
    bool collectPoint( int x, int y );

  private:
    bool readBoardSize( QFile & map );
    void initBlocksToWalls();
    bool readBoardBlocks( QFile & map );
    void addWallBlock( int x, int y );
    void addGrassBlock( int x, int y );
    void addFloorBlock(
      int x, int y, GLuint texture, const Material & material
    );

  private:
    QString name;
    int width;
    int height;
    int** blocks;
    QPointF player1Start;
    QVector< QPointF > ghostStarts;
    QList< QPointF > pacDots;

    Material grassMaterial;
    Material wallMaterial;
    Material dotsMaterial;
    Material defaultMaterial;

    GLuint staticList;
    GLUquadric* dotQuadric;
    GLuint dotList;
    GLuint wallTexture;
    GLuint roofTexture;
    GLuint grassTexture;
    GLuint ghostsStartTexture;
    GLuint dotTexture;

    enum BlockTypes {
      Wall, Dot, UsedDot, Path, Powerup, Teleport1,
      PlayerStart, GhostsStart, PlayerWall
    };
};

#endif

