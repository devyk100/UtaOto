//
// Created by yash on 7/11/24.
//

#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#include <portaudio.h>

namespace  audioFile
{
    class AudioFile
    {
        public:
            AudioFile(const char*, PaStream*);
            bool play();
            bool pause();
            bool stop();
            ~AudioFile();
        private:
            const char* filename;
            PaStream* stream;
    };
};

#endif //AUDIOFILE_H
