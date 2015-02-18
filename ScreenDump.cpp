#include "ScreenDump.hpp"
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>

#include <fstream>

ScreenDump::ScreenDump( int width, int height )
	:
	m_width( width ),
	m_height( height )
{
	createTextures();
}

ScreenDump::~ScreenDump()
{
}

void ScreenDump::bindData(int width, int height)
{

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_prevFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer); // Reset framebuffer binding
    glClearColor(-1, -1, -1, -1);
    glClear(GL_COLOR_BUFFER_BIT);
	
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
	if( (width != m_width) || (height != m_height) ){
		m_width = width;
		m_height = height;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0,GL_RGBA, GL_FLOAT, NULL);
	}
}

void ScreenDump::unbindData()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_prevFrameBuffer);
}

void ScreenDump::createTextures()
{

    glGenTextures(1, &m_renderedTexture);
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0); 

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0,GL_RGBA, GL_FLOAT, NULL);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_prevFrameBuffer);
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderedTexture, 0);

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"\nCould not set up screen dump!";
    glBindFramebuffer(GL_FRAMEBUFFER, m_prevFrameBuffer);
    
}

std::vector<float> ScreenDump::getDumpTexImage() {
    std::vector<float> voxeldumpvec(4*m_width*m_height);
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &voxeldumpvec[0]);
    glBindTexture(GL_TEXTURE_2D, 0);
    return voxeldumpvec;
}

void ScreenDump::dumpTexImage(std::string filename) {

    std::vector<float> voxeldumpvec = getDumpTexImage();

    std::ofstream ofs;
    if (filename.empty()) {
         ofs.open("dump.dat", std::ios::out | std::ios::binary);
    }
    else {
        ofs.open(filename, std::ios::out | std::ios::binary);
    }

    double val;
    val = m_width;
    ofs.write(reinterpret_cast<const char*>(&val),sizeof(double));
    val = m_height;
    ofs.write(reinterpret_cast<const char*>(&val),sizeof(double));
    for (size_t i = 0; i<voxeldumpvec.size(); i++) {
        val = (double)voxeldumpvec[i];
        ofs.write(reinterpret_cast<const char*>(&val),sizeof(double));
    }
    ofs.close();

}
