#include <QGLWidget>

class PacMan : public QObject {
  Q_OBJECT

  public:
    PacMan();
    ~PacMan();
    void initializeGL( QGLWidget & target );
    void render( QGLWidget & target );

  private:
    void addSphere( int dTheta, int dPhi );
    void addSphereVertex( int theta, int phi );

  private:
    GLUquadric* sphereQuadric;
    GLuint sphereList;
    GLuint pacmanTexture;
};

