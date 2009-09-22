#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <QWidget>

class Game;
class QGLWidget;
class QHBoxLayout;

class GameWindow : public QWidget {
  Q_OBJECT

  public:
    GameWindow( QWidget* parent );
    ~GameWindow();
    bool initialize();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    virtual void keyPressEvent( QKeyEvent* event );

  private:
    QHBoxLayout* layout;
    Game* game;
    QSize lastSize;
};

#endif

