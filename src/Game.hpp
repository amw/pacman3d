#ifndef GAME_HPP
#define GAME_HPP

#include "GameBoard.hpp"
#include "PacMan.hpp"
#include <QGLWidget>
#include <QTime>

#define FOVY 45.0f
#define MS_TO_REPORT_FPS 1000

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
    void printFpsReport();
    void refreshCamera();

  private:
    QTime lastFpsReport;
    int framesRenderedSinceLastReport;
    GameBoard board;
    PacMan hero;
    QColor background;
    double aspectRatio;
    bool centerCamera;
    bool isometricCamera;
    bool followingCamera;
};

#endif

