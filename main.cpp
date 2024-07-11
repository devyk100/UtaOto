#include <iostream>
#include <portaudio.h>

#include "include/audio.h"

#define SAMPLE_RATE (44100)
struct paTestData
{
    float left_phase;
    float right_phase;
} data;

// static paTestData data;

static int patestCallback(const void* inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData )
{
    // auto *data = (paTestData*)userData;
    auto *data = static_cast<paTestData*>(userData);
    auto out = static_cast<float*>(outputBuffer);
    (void) inputBuffer;
    data -> left_phase = -1.0f;
    data->right_phase = -1.0f;

    int sec = static_cast<int>(timeInfo->currentTime);
    for(unsigned int i = 0; i < framesPerBuffer; i++)
    {
        *(out++) = data->left_phase;
        *(out++) = data->right_phase;

        // std::cout << timeInfo;

        data->right_phase += 0.015f*(float)((sec%2)*3 + 1);
        data->left_phase += 0.2f;

        if(data->left_phase >= 1.0f) data->left_phase -= 2.0f;
        if(data->right_phase >= 1.0f) data->right_phase -= 2.0f;
    }
    std::cout << timeInfo->currentTime << " ";
    return 0;
}

void errCheck(PaError err)
{
    if(err != paNoError)
    {
        printf("Error occured: %s\n", Pa_GetErrorText(err));
    }
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    PaStream *stream;
    PaError err;
    audio::PaInitialize a;
    errCheck(err);
    data.left_phase = -1.0f;
    data.right_phase = -1.0f;
    err = Pa_OpenDefaultStream(
        &stream, 0, 2, paFloat32, SAMPLE_RATE, 256, patestCallback, &data);
    errCheck(err);

    err = Pa_StartStream(stream);
    errCheck(err);
    Pa_Sleep(1000*10);
    err = Pa_StopStream(stream);
    errCheck(err);
    err = Pa_CloseStream(stream);
    errCheck(err);
    return 0;
}
