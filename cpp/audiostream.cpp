//
// Created by yash on 7/11/24.
//

#include "../include/audiostream.h"

#include <cstring>
#include <iostream>

    // support for modified file buffers, as well as synthesizers are to be added.
audioStream::BufferState::BufferState(PaStream* stream):bufferSize(BUFFER_SIZE), noOfChannels(NO_OF_CHANNELS), position(0), sampleRate(-1), isPaused(false)
{
    err = Pa_Initialize();
    prevBuffer = new float[bufferSize * noOfChannels];
    currBuffer = new float[bufferSize * noOfChannels];
    nextBuffer = new float[bufferSize * noOfChannels];
    nextNextBuffer = new float[bufferSize * noOfChannels];
    temp = new float[bufferSize * noOfChannels];
}

void audioStream::BufferState::addFile(const char* filename)
{
    SF_INFO sfInfo;
    SNDFILE* sndFile = sf_open(filename, SFM_READ, &sfInfo);
    checkError(sndFile);
    if(sfInfo.channels != 2 && sfInfo.channels != 1)
    {
        std::cerr << "This audio file does not have a channel or more than 2 are not supported in this program\n";
        return;
    }
    if(sampleRate == -1) sampleRate = sfInfo.samplerate;
    if(sampleRate != sfInfo.samplerate)
    {
        std::cerr << "The audio files do not have a uniform sample rate\n";
        return;
    }
    FileChannels.push_back({sfInfo, sndFile});
}

void audioStream::BufferState::checkError(SNDFILE* sndfile)
{
    if(!sndfile)
    {
        printf("Error occured while reading the file\n");
        std::cerr << "Error in reading the file\n";
    }
}

sf_count_t audioStream::BufferState::startPlayback()
{
    isPaused = false;
    sf_count_t readCount;
    memset(currBuffer, 0, noOfChannels*bufferSize*sizeof(float));
    for(auto it: FileChannels)
    {
        readCount = sf_read_float(it.snfFile, temp, bufferSize);
        if(readCount != bufferSize)
        {
            std::cout << "Read count "<<readCount<< ", buffer size " <<bufferSize<<std::endl;
        }
        for(int i = 0; i < bufferSize*noOfChannels; i++)
        {
            currBuffer[i] += temp[i];
        }
        memset(temp, 0, noOfChannels*bufferSize*sizeof(float));
    }
    nextNextFetcherThread = std::thread([this](){fetchNextBuffers(true);});
    return 0;
}

sf_count_t audioStream::BufferState::fetchNextBuffers(bool &&isStartPlayback)
{
    memset(nextNextBuffer, 0, noOfChannels*bufferSize*sizeof(float));
    sf_count_t readCount;
    for(auto it:FileChannels)
    {
        readCount = sf_read_float(it.snfFile, temp, bufferSize);
        if(readCount != bufferSize)
        {
            // file read complete, or some error;
        }
        for(int i = 0; i < bufferSize; i++)
        {
            nextNextBuffer[i] += temp[i];
        }
        memset(temp, 0, noOfChannels*bufferSize*sizeof(float));
    }
    if(isStartPlayback)
    {
        // fill the next, and empty the nextnext, replace pointers;
        float* tempPt = nextBuffer;
        nextBuffer = nextNextBuffer;
        nextNextBuffer = tempPt;
        fetchNextBuffers(false);
        resumePlayback();
    }
    return 0;
}

sf_count_t audioStream::BufferState::refreshBuffers()
{
    if(nextNextFetcherThread.joinable())
    {
        nextNextFetcherThread.join();
    }
    float* tempPt = prevBuffer;
    prevBuffer = currBuffer;
    currBuffer = nextBuffer;
    nextBuffer = nextNextBuffer;
    nextNextBuffer = tempPt;
    position = 0;
    nextNextFetcherThread = std::thread([this](){fetchNextBuffers(false);});
    return 0;
}

audioStream::BufferState::~BufferState()
{
    delete[] temp;
    delete[] currBuffer;
    delete[] nextBuffer;
    delete[] nextNextBuffer;
    Pa_Terminate();
}

sf_count_t audioStream::BufferState::pausePlayback()
{
    Pa_StopStream(stream);
    return 0;
}

sf_count_t audioStream::BufferState::resumePlayback()
{
    Pa_StartStream(stream);
    return 0;
}

int audioStream::BufferState::defaultAudioStreamCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    auto data = static_cast<BufferState*>(userData);
    auto out = static_cast<float*>(outputBuffer);
    static_cast<void>(inputBuffer);

    for(unsigned int i = 0; i < framesPerBuffer; i++)
    {
        if(data->isPaused)
        {
            *(out++) = -1.0f;
            *(out++) = -1.0f;
            return paContinue;
        }

        if(data->position < data->bufferSize)
        {
            *(out++) = data->currBuffer[data->position++];
            *(out++) = data->currBuffer[data->position++];
            // std::cout << data->position << " / " << data->bufferSize << std::endl;
        }
        else
        {
            *(out++) = 0.0f;
            *(out++) = 0.0f;
            data->refreshBuffers();
            std::cout << data->position << " / " << data->bufferSize;
            // std::cout << "End of the buffer reached\n";
        }
    }
    return paContinue;
}


sf_count_t audioStream::BufferState::openDefaultStream()
{
    if(CustomCallback.isCustom)
    {
        Pa_OpenDefaultStream(&stream, 0, static_cast<int>(noOfChannels), paFloat32, (double)sampleRate, 512, CustomCallback.callback, CustomCallback.data);
    }
    else
    {
        Pa_OpenDefaultStream(&stream, 0, static_cast<int>(noOfChannels), paFloat32, (double)sampleRate, 512, defaultAudioStreamCallback, this);
    }
    return paContinue;
}

sf_count_t audioStream::BufferState::closeStream()
{
    Pa_CloseStream(stream);
    return paComplete;
}

void audioStream::BufferState::checkError(PaError err)
{
    if(err != paNoError)
    {
        const std::string errString = std::string("Error occured ") + Pa_GetErrorText(err);
        this->~BufferState();
        throw std::runtime_error(errString);
    }
}

bool audioStream::BufferState::setCustomCallback(int (*callback)(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*), void* data)
{
    this->CustomCallback.callback = callback;
    this->CustomCallback.isCustom = true;
    this->CustomCallback.data = data;
    return true;
}

