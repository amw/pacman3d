#include <QGLWidget>
#include <QString>
#include <QFile>

#define BLOCK_WALL 'X'
#define BLOCK_DOT '.'
#define BLOCK_PATH ' '
#define BLOCK_POWERUP 'S'
#define BLOCK_TELEPORT_1 '1'
#define BLOCK_PLAYER_START '_'
#define BLOCK_GHOSTS_START 'G'
#define BLOCK_PLAYER_WALL '='

#define BLOCK_WIDTH 0.2f

class GameBoard : QObject {
  Q_OBJECT

  public:
    GameBoard( QString name );
    ~GameBoard();
    bool initialize();
    void initializeGL();
    void render();

  private:
    bool readBoardSize( QFile & map );
    void initBlocksToWalls();
    bool readBoardBlocks( QFile & map );
    void addWallBlock( double x, double y );

  private:
    QString name;
    int width;
    int height;
    int** blocks;
    bool allocatedBlocks;
    GLuint wallsList;

    enum BlockTypes {
      Wall, Dot, Path, Powerup, Teleport1, PlayerStart, GhostsStart, PlayerWall
    };
};

