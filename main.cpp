#include <cstring>
#include <iostream>
#include <portaudio.h>
#include <sndfile.h>
#include "include/audiostream.h"
#include "include/synthesizer.h"


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
    synthesizer::Notes.reserve(40);
    PaStream *stream = nullptr;
    audioStream::BufferState fdata(stream);
    fdata.addFile(filename);
    fdata.addFile("../audio2.mp3");
    fdata.addFile("../audio3.mp3");
    auto *a = new synthesizer::Synthesizer(&fdata);
    // synthesizer::PianoNote pianoNote(5000.0, SAMPLE_RATE);
    // fdata.setCustomCallback(synthesizer::audioCallback, &pianoNote);
    fdata.openDefaultStream();

    fdata.startPlayback();

    char k;

    std::cin >> k;
    std::cout << "After sleep\n";

    fdata.pausePlayback();
    fdata.closeStream();

    return 0;
}
