#include <QGLWidget>
#include <QSize>

class QKeyEvent;

class GameWindow : public QGLWidget {
  Q_OBJECT

  public:
    GameWindow( QWidget* parent );
    ~GameWindow();
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
    QColor background;
};

