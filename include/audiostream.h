//
// Created by yash on 7/11/24.
//

#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H
#define BUFFER_SIZE (1024*400)
#define NO_OF_CHANNELS (2)
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

    class BufferState
    {
        private:
            float* prevBuffer, *nextBuffer, *nextNextBuffer, *temp;

            std::pmr::vector<FileChannel> FileChannels;

            // std::pmr::vector<std::pmr::vector<float*>> tempBuffers;
        // support for modified file buffers, as well as synthesizers are to be added.
        public:
            sf_count_t position;
            sf_count_t noOfChannels;
            sf_count_t bufferSize;
            sf_count_t fetchNextBuffers(bool &&isStartPlayback);
            std::thread nextNextFetcherThread;
        long int sampleRate;
            float *currBuffer;
            BufferState();
            void addFile(const char* filename);
            static void checkError(SNDFILE* sndfile);
            sf_count_t startPlayback();
            sf_count_t refreshBuffers();
            ~BufferState();

    };

    struct bufferData
    {
        float* audioBuffer;
        sf_count_t bufferSize = 1024;
        sf_count_t position;
    };

    int audioStreamCallback(const void* inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);



};

#endif //AUDIOSTREAM_H
