//
// Created by yash on 7/11/24.
//

#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H
#define BUFFER_SIZE (1024*400)
#define NO_OF_CHANNELS (2)
#define SAMPLE_RATE (44100)
#include <functional>
#include <portaudio.h>
#include <sndfile.h>
#include <thread>
#include <vector>

namespace audioStream
{
    struct FileChannel
    {
        SF_INFO sf_info;
        SNDFILE* snfFile;
    };

    struct customCallback
    {
        bool isCustom = false;
        int(*callback)(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
        void* data;
    };

    class BufferState
    {
        private:
            customCallback CustomCallback;
            PaError err;
            float* prevBuffer, *nextBuffer, *nextNextBuffer, *temp;
            PaStream* stream;
            std::pmr::vector<FileChannel> FileChannels;
            static int defaultAudioStreamCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
            // std::pmr::vector<std::pmr::vector<float*>> tempBuffers;
        // support for modified file buffers, as well as synthesizers are to be added.
        public:
            sf_count_t position;
            bool isPaused;
            sf_count_t noOfChannels;
            sf_count_t bufferSize;
            sf_count_t fetchNextBuffers(bool &&isStartPlayback);
            std::thread nextNextFetcherThread;
            long int sampleRate;
            float *currBuffer;
            explicit BufferState(PaStream *stream);
            void addFile(const char* filename);
            static void checkError(SNDFILE* sndfile);
            void checkError(PaError err);
            sf_count_t openDefaultStream();
            sf_count_t closeStream();
            sf_count_t startPlayback();
            sf_count_t refreshBuffers();
            sf_count_t pausePlayback();
            sf_count_t resumePlayback();
            bool setCustomCallback(int(*callback)(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*), void* data);
            ~BufferState();
    };

};

#endif //AUDIOSTREAM_H
