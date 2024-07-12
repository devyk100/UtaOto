#include <cstring>
#include <iostream>
#include <portaudio.h>
#include <sndfile.h>
#include "include/audio.h"
#include "include/audiostream.h"
#include "include/utils.h"

#define SAMPLE_RATE (44100)


// static audioStream::paTestData data;


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
    audioStream::BufferState fdata;
    fdata.addFile(filename);
    fdata.addFile("../audio2.mp3");
    fdata.addFile("../audio3.mp3");

    PaStream *stream = nullptr;
    PaStreamParameters p;
    PaError err;
    err = Pa_Initialize();
    errCheck(err);
    fdata.startPlayback();
    err = Pa_OpenDefaultStream(&stream, 0, (int)fdata.noOfChannels, paFloat32, (double)fdata.sampleRate, 256, audioStream::audioStreamCallback, &fdata);

    // err = Pa_WriteStream(stream, fdata.audioBuffer, sfInfo.frames);
    errCheck(err);

    errCheck(err);
    err = Pa_StartStream(stream);
    // err = Pa_WriteStream(stream, fdata.audioBuffer, written);
    // int BUFFER_LEN = sfInfo.frames * sfInfo.channels;
    // sf_count_t readCount = 0;
    // float data[BUFFER_LEN];
    // int subFormat = sfInfo.format;
    // // ::memset(data, 0, sizeof(data));
    // while ((readCount = sf_read_float(sndFile, fdata.audioBuffer, fdata.bufferSize)))
    // {
    //     if (subFormat == SF_FORMAT_FLOAT || subFormat == SF_FORMAT_DOUBLE)
    //     {
    //         int m = 0;
    //         for (m = 0 ; m < readCount ; ++m)
    //         {
    //             data[m] *= 1.0f;
    //         }
    //     }
    //     err = Pa_WriteStream(stream, data, BUFFER_LEN);
    //     errCheck(err);
    //     ::memset(data, 0, sizeof(data));
    // }

    // sf_close(sndFile);
    errCheck(err);

    std::cin.get();
    std::cout << "After sleep\n";
    err = Pa_StopStream(stream);
    errCheck(err);
    err = Pa_CloseStream(stream);
    errCheck(err);
    err = Pa_Terminate();
    return 0;
}
