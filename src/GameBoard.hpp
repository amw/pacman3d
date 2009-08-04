#include <QGLWidget>
#include <QString>

class GameBoard : QObject {
  Q_OBJECT

  public:
    GameBoard( QString name );
    ~GameBoard();
    bool initialize();
    void initializeGL();
    void render();

  private:
    QString name;
    GLuint wallsList;
};

