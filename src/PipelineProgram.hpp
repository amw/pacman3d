#include <QObject>
#include <QVector>
#include <QtOpenGL>

class PipelineProgram : public QObject {
  Q_OBJECT

  public:
    PipelineProgram();
    ~PipelineProgram();
    bool attachShader( GLenum type, QString filename );
    bool link();
    bool isLinked();
    bool enable( bool enable );
    GLint getUniformLocation( char * name );

  private:
    void printShaderLog( GLuint shader );
    void printLog();

  private:
    GLuint id;
    QVector< GLuint > shaders;
};

