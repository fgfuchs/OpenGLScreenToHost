# OpenGLScreenToHost
class allowing to render to a texture and save the texture to a file (without intervention in the shaders)

USAGE:

1) Include the header

    #include "ScreenDump.hpp"

2) Create an instance of the class

    ScreenDump screenDump(width,height);

3) Use bool for triggering

    bool dump_toFile;

4) Do the following in the render loop:

    if (dump_toFile) {
        screenDump->bindData(width, height); // width, height of the screen
    }

    draw();

    if (dump_toFile) {
        screenDump->dumpTexImage(fileName);
        screenDump->unbindData();
        dump_toFile = false;
    }
