#include <cstring>
#include <iostream>
#include <portaudio.h>
#include <sndfile.h>
#include "include/audiostream.h"


void errCheck(PaError err)
{
    if(err != paNoError)
    {
        printf("Error occured: %s\n", Pa_GetErrorText(err));
    }
}

const char* filename = "../audio.mp3";

int main()
{
    PaStream *stream = nullptr;
    audioStream::BufferState fdata(stream);
    fdata.addFile(filename);
    fdata.addFile("../audio2.mp3");
    fdata.addFile("../audio3.mp3");

    fdata.openDefaultStream();
    fdata.startPlayback();

    char k;

    std::cin >> k;
    std::cout << "After sleep\n";

    fdata.pausePlayback();
    fdata.closeStream();

    return 0;
}
