#include <QObject>
#include <QVector>
#include "gl.h"

class PipelineProgram : public QObject {
  Q_OBJECT

  public:
    PipelineProgram();
    ~PipelineProgram();
    bool attachShader( GLenum type, QString filename );
    bool link();
    bool isLinked();
    bool enable( bool enable );
    GLint getUniformLocation( const char * name );

  private:
    void printShaderLog( GLuint shader );
    void printLog();

  private:
    GLuint id;
    QVector< GLuint > shaders;
};

