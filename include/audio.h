//
// Created by yash on 7/11/24.
//

#ifndef AUDIO_H
#define AUDIO_H
#include <cstdio>
#include <portaudio.h>

namespace audio
{
    struct PaInitialize
    {
        PaError err;
        PaInitialize()
        {
            err = Pa_Initialize();
            checkErr(err);
        }
        ~PaInitialize()
        {
            err = Pa_Terminate();
            checkErr(err);
        }
        static void checkErr(const PaError err)
        {
            if(err != paNoError)
            {
                printf("Error occured: %s\n", Pa_GetErrorText(err));
            }
        }
    };
};


#endif //AUDIO_H
