//
// Created by yash on 7/11/24.
//
#include "../include/utils.h"

#include <iostream>
// namespace audioStream
// {
//     int patestCallback(const void* inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData )
//     {
//         // auto *data = (paTestData*)userData;
//         auto *data = static_cast<paTestData*>(userData);
//         auto out = static_cast<float*>(outputBuffer);
//         (void) inputBuffer;
//         data -> left_phase = -1.0f;
//         data->right_phase = -1.0f;
//
//         int sec = static_cast<int>(timeInfo->currentTime);
//         for(unsigned int i = 0; i < framesPerBuffer; i++)
//         {
//             *(out++) = data->left_phase;
//             *(out++) = data->right_phase;
//
//             // std::cout << timeInfo;
//
//             data->right_phase += 0.015f*(float)((sec%2)*3 + 1);
//             data->left_phase += 0.2f;
//
//             if(data->left_phase >= 1.0f) data->left_phase -= 2.0f;
//             if(data->right_phase >= 1.0f) data->right_phase -= 2.0f;
//         }
//         std::cout << timeInfo->currentTime << " ";
//         return 0;
//     }
//
//
//     int playFileCallback(const void* inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* fdata)
//     {
//         auto data = static_cast<fileData*>(fdata);
//         float* out = static_cast<float*>(outputBuffer);
//         unsigned int i;
//
//         for(i = 0; i < framesPerBuffer; i++)
//         {
//             if(data->position < data->bufferSize)
//             {
//                 *(out++) = data->audioBuffer[data->position++];
//                 *(out++) = data->audioBuffer[data->position++];
//             }
//             else
//             {
//                 *(out++) = 0.5f;
//                 *(out++) = 0.5f;
//             }
//         }
//         return paContinue;
//     }
//
// };