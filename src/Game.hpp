#ifndef GAME_HPP
#define GAME_HPP

#include "GameBoard.hpp"
#include "PacMan.hpp"
#include "Ghost.hpp"
#include "Light.hpp"
#include "PipelineProgram.hpp"
#include <QGLWidget>
#include <QTime>
#include <QTimer>

#define FOVY 45.0f
#define MS_TO_REPORT_FPS 1000
#define GHOSTS_COUNT 4
#define INVULNERABILITY_TIME 3500

class QKeyEvent;

class Game : public QGLWidget {
  Q_OBJECT

  public:
    Game( QWidget* parent );
    ~Game();
    virtual void keyPressEvent( QKeyEvent* event );
    bool initialize();
    void start();

  public slots:
    void playerCollectedPoint();

  protected:
    void initializeGL();
    void resizeGL( int w, int h );
    void paintGL();

  private:
    void paintWithMotionBlur( int timeStep );
    void paintFrame( int timeStep );
    void updatePositions( int timeStep );
    void printFpsReport();
    void refreshCamera();
    void initializeShaders();
    void initializeLights();
    void prepareLights();
    int prepareMainLight( int light );

  private:
    GameBoard board;
    PacMan hero;
    Ghost* ghosts[ GHOSTS_COUNT ];
    int lifes;
    int points;
    bool paused;

    bool shadersAreSupported;
    bool usingShaders;
    PipelineProgram * shaderProgram;
    GLint lightsCountLocation;

    QTimer timer;
    bool needsRepaint;

    int motionBlurFrames;
    QTime lastFrame;
    QTime lastFpsReport;
    QTime lastPlayerDeath;
    int framesRenderedSinceLastReport;

    QColor background;

    double aspectRatio;

    bool centerCamera;
    bool isometricCamera;
    bool followingCamera;
    double cameraZoom;

    float ambientLight[ 4 ];
    Light ghostStartsLight;
};

#endif

