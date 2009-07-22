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
    virtual void keyPressEvent( QKeyEvent* event );

  private:
    QHBoxLayout* layout;
    Game* game;
};

