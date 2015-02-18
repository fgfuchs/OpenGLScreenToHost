#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>

class ScreenDump {
public:
    
    ScreenDump( int width, int height );
    ~ScreenDump();

    void bindData(int width, int height);
    void unbindData();
    std::vector<float> getDumpTexImage();
    void dumpTexImage(std::string filename);

private:

    void createTextures();
    
    int m_width;
    int m_height;

    GLint  m_prevFrameBuffer;
    GLuint m_frameBuffer;
    GLuint m_renderedTexture;

}; 
