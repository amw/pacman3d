#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

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
    QPoint getSize();
    QPointF getRealSize();
    QPointF getPlayer1Start();
    bool isAccessibleByPlayer( int x, int y );
    bool isAccessibleByGhost( int x, int y );

  private:
    bool readBoardSize( QFile & map );
    void initBlocksToWalls();
    bool readBoardBlocks( QFile & map );
    void addWallBlock( int x, int y );
    void addGrass();
    void addFloorBlock( int x, int y, GLuint texture );

  private:
    QString name;
    int width;
    int height;
    int** blocks;
    bool allocatedBlocks;
    QPoint player1Start;
    QVector< QPoint > ghostStarts;

    GLuint wallsList;
    GLuint wallTexture;
    GLuint roofTexture;
    GLuint grassTexture;
    GLuint ghostsStartTexture;

    enum BlockTypes {
      Wall, Dot, Path, Powerup, Teleport1, PlayerStart, GhostsStart, PlayerWall
    };
};

#endif

