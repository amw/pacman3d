#include "GameBoard.hpp"
#include "PacMan.hpp"
#include <QGLWidget>

class QKeyEvent;

class Game : public QGLWidget {
  Q_OBJECT

  public:
    Game( QWidget* parent );
    ~Game();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    virtual void keyPressEvent( QKeyEvent* event );
    bool initialize();
    void start();

  protected:
    void initializeGL();
    void resizeGL( int w, int h );
    void paintGL();

  private:
    GameBoard board;
    PacMan hero;
    QColor background;
    double x, y, z;
};

